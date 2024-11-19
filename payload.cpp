

#include <thread>

#include "payload.hpp"
#include "interface.hpp"

#include "helpers.hpp"

std::thread payload_start_portal_gio_mainloop_thread(){
  auto& interface_singleton = InterfaceSingleton::getSingleton();
  auto* portal_handle = interface_singleton.portal_handle.load();
  // this thread is going to be trapped in the gio mainloop
  std::thread portal_gio_mainloop_thread = std::thread(
    [portal_handle](){
      g_main_loop_run(portal_handle->gio_mainloop);
    }
  );
  // wait until xdpsession is up
  // TODO: this CAN actually fail and trap the program...
  // TODO: but i think it's relatively safe to assume that the session will be up for now
  // TODO: eventually need to deal with this
  while(!portal_handle->session.load()){
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  return std::move(portal_gio_mainloop_thread);
}


std::thread payload_start_pipewire_thread(){
  auto& interface_singleton = InterfaceSingleton::getSingleton();
  auto* pipewire_handle = interface_singleton.pipewire_handle.load();
  // this thread is going to be trapped in the pipewire mainloop
  std::thread pipewire_thread = std::thread(
    [](){
      auto& interface_singleton = InterfaceSingleton::getSingleton();
      while (interface_singleton.interface_handle.load()->pw_stop_flag.load() == false) {
        auto* pipewire_handle = interface_singleton.pipewire_handle.load();
        pw_loop_iterate(pw_main_loop_get_loop(pipewire_handle->pw_mainloop), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
      }
      fprintf(stderr, "%s", green_text("[payload] pw stop signal received. pw stopped. \n").c_str());
    }
  );
  fprintf(stderr, "%s", green_text("[payload] pipewire thread started.\n").c_str());
  return std::move(pipewire_thread);
}


// this payload_main will be executed by the payload_thread, which is created in XShmAttachHook
// and this thread will be detached immediately after creation
void payload_main(){


  auto& interface_singleton = InterfaceSingleton::getSingleton();
  auto* portal_handle = interface_singleton.portal_handle.load();

  // start the gio mainloop thread
  std::thread portal_gio_mainloop_thread = payload_start_portal_gio_mainloop_thread();

  // start screencast session
  // this will get the pipewire fd into the portal object
  xdp_session_start(
    portal_handle->session.load(), NULL, NULL,
    XdpScreencastPortal::screencast_session_start_cb,
    portal_handle
  );
  
  // wait until pipewire_fd is up
  while(portal_handle->status.load() == XdpScreencastPortalStatus::kInit ) {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  };

  // screencast cancelled. we stop the gio mainloop and join the gio mainloop thread
  if (portal_handle->status.load() == XdpScreencastPortalStatus::kCancelled) {
    fprintf(stderr, "%s", red_text("[payload] screencast cancelled. stop gio and join gio thread. \n").c_str());
    g_main_loop_quit(portal_handle->gio_mainloop);
    portal_gio_mainloop_thread.join();
    return;
  }

  while(portal_handle->pipewire_fd.load() == -1){
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  fprintf(stderr, "%s", green_text("[payload SYNC] pipewire_fd acquired: " + std::to_string(portal_handle->pipewire_fd.load()) + "\n").c_str());

  
  while(interface_singleton.pipewire_handle.load() == nullptr){
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  fprintf(stderr, "%s", green_text("[payload SYNC] got pipewire_handle.\n").c_str());

  // start the pipewire thread
  std::thread pipewire_thread = payload_start_pipewire_thread();

  pipewire_thread.join();
  interface_singleton.interface_handle.load()->payload_pw_stop_confirm.store(true, std::memory_order_seq_cst);
  fprintf(stderr, "%s", green_text("[payload SYNC] pw stop confirmed.\n").c_str());

  portal_gio_mainloop_thread.join();
  interface_singleton.interface_handle.load()->payload_gio_stop_confirm.store(true, std::memory_order_seq_cst);
  fprintf(stderr, "%s", green_text("[payload SYNC] gio stop confirmed.\n").c_str());

  
  return;

}