
#include <thread>
#include <chrono>


void XShmAttachHook();
void XShmDetachHook();

int main(){
  XShmAttachHook();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  XShmDetachHook();

  std::this_thread::sleep_for(std::chrono::seconds(3));

  XShmAttachHook();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  XShmDetachHook();
}