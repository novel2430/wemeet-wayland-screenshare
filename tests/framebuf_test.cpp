#include "format.hpp"
#include "framebuf.hpp"


int main(){
  auto framebuf = FrameBuffer{1080, 1920, SpaVideoFormat_e::RGBx};
  
  printf("framebuf data_size: %ld\n", framebuf.data_size);
  printf("framebuf height: %d\n", framebuf.height);
  printf("framebuf width: %d\n", framebuf.width);
  printf("framebuf row_byte_stride: %d\n", framebuf.row_byte_stride);
  printf("framebuf format: %d\n", static_cast<int>(framebuf.format));

  // framebuf.blocking_imshow();

  framebuf.update_param(1440, 2560, SpaVideoFormat_e::BGR);

  printf("framebuf data_size: %ld\n", framebuf.data_size);
  printf("framebuf height: %d\n", framebuf.height);
  printf("framebuf width: %d\n", framebuf.width);
  printf("framebuf row_byte_stride: %d\n", framebuf.row_byte_stride);
  printf("framebuf format: %d\n", static_cast<int>(framebuf.format));


  // framebuf.blocking_imshow();

}