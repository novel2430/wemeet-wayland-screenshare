#include <cstdlib>
#include <unistd.h>
#include <stdexcept>
#include <errno.h>


void call_with_std_system()
{
  std::system(
    "krfb-virtualmonitor --resolution 1920x1080 --name vv --password 123 --port 12345"
  );
}

void call_with_execvp(){
  char* args[] = {
    (char*) "krfb-virtualmonitor",
    (char*) "--resolution",
    (char*) "1920x1080",
    (char*) "--name",
    (char*) "wemeetapp-virtualmonitor",
    (char*) "--password",
    (char*) "123456",
    (char*) "--port",
    (char*) "54321",
    NULL  // this NULL is a must. dang it.
  };
  int errcode = execvp(
    args[0],
    args
  );

  printf("control reached a wrong place\n");
  printf("errcode is %d\n", errcode);
  
  // get the error code
  printf("errno is %d\n", errno);



  // control should never reach here
  throw std::runtime_error("execvp failed");
}

int main(){


call_with_execvp();


}