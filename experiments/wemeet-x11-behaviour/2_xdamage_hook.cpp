#include <cstdio>
#include "original_funcs.hpp"

#include "X11/Xlib.h"
#include "X11/Xlibint.h"


bool initial_shm_getimage_reported = false;

extern "C"
{
  
// we first hookup XShmAttach
// this is expected to be called when you start screencast
Bool XShmAttach(Display* dpy, XShmSegmentInfo* shminfo)
{
  fprintf(stderr, "wemeetapp called XShmAttach(%p, %p)\n", dpy, shminfo);
  fprintf(stderr, "now we are printing the info about this xdisplay\n");
  fprintf(stderr, "dpy->fd: %d\n", dpy->fd);
  fprintf(stderr, "dpy->proto_major_version: %d\n", dpy->proto_major_version);
  fprintf(stderr, "dpy->proto_minor_version: %d\n", dpy->proto_minor_version);
  fprintf(stderr, "dpy->vendor: %s\n", dpy->vendor);
  fprintf(stderr, "dpy->qlen: %d\n", dpy->qlen);
  fprintf(stderr, "dpy->last_request_read: %lu\n", dpy->last_request_read);
  fprintf(stderr, "dpy->request: %lu\n", dpy->request);
  fprintf(stderr, "dpy->max_request_size: %u\n", dpy->max_request_size);
  fprintf(stderr, "dpy->display_name: %s\n", dpy->display_name);
  fprintf(stderr, "dpy->default_screen: %d\n", dpy->default_screen);
  fprintf(stderr, "dpy->nscreens: %d\n", dpy->nscreens);
  fprintf(stderr, "dpy->motion_buffer: %lu\n", dpy->motion_buffer);
  fprintf(stderr, "dpy->min_keycode: %d\n", dpy->min_keycode);
  fprintf(stderr, "dpy->max_keycode: %d\n", dpy->max_keycode);
  fprintf(stderr, "dpy->xdefaults: %s\n", dpy->xdefaults);
  fprintf(stderr, "dpy->screens: %p\n", dpy->screens);
  
  fprintf(stderr, "and now we are going to print some more info about shminfo\n");
  fprintf(stderr, "shminfo->shmid: %d\n", shminfo->shmid);
  fprintf(stderr, "shminfo->shmaddr: %p\n", shminfo->shmaddr);
  fprintf(stderr, "shminfo->readOnly: %d\n", shminfo->readOnly);
  
  
  Bool ret = XShmAttachFunc(dpy, shminfo);
  return ret;
}

// then we hookup XShmDetach
// this is expected to be called when you stop screencast
Bool XShmDetach(Display* dpy, XShmSegmentInfo* shminfo)
{
  fprintf(stderr, "wemeetapp called XShmDetach(%p, %p)\n", dpy, shminfo);
  Bool ret = XShmDetachFunc(dpy, shminfo);
  return ret;
}

// and finally the main function XShmGetImage
// this is expected to be called when wemeetapp want to get the image
Bool XShmGetImage(Display* dpy, Drawable d, XImage* image, int x, int y, unsigned long plane_mask)
{
  fprintf(stderr, "wemeetapp called XShmGetImage(%p, %lu, %p, %d, %d, %lu)\n", dpy, d, image, x, y, plane_mask);
  
if (!initial_shm_getimage_reported) {
  
  fprintf(stderr, "image->width: %d\n", image->width);
  fprintf(stderr, "image->height: %d\n", image->height);
  fprintf(stderr, "image->xoffset: %d\n", image->xoffset);
  fprintf(stderr, "image->format: %d\n", image->format);
  fprintf(stderr, "image->data: %p\n", image->data);
  fprintf(stderr, "image->byte_order: %d\n", image->byte_order);
  fprintf(stderr, "image->bitmap_unit: %d\n", image->bitmap_unit);
  fprintf(stderr, "image->bitmap_bit_order: %d\n", image->bitmap_bit_order);
  fprintf(stderr, "image->bitmap_pad: %d\n", image->bitmap_pad);
  fprintf(stderr, "image->depth: %d\n", image->depth);
  fprintf(stderr, "image->bytes_per_line: %d\n", image->bytes_per_line);
  fprintf(stderr, "image->bits_per_pixel: %d\n", image->bits_per_pixel);
  initial_shm_getimage_reported = true;
}
  
  Bool ret = XShmGetImageFunc(dpy, d, image, x, y, plane_mask);
  return ret;
}

Bool XDamageQueryExtension(Display *dpy, int *event_base_return, int *error_base_return) {
  fprintf(stderr, "XDamageQueryExtension(%p, %p, %p)\n", dpy, event_base_return, error_base_return);
  auto ret = XDamageQueryExtensionFunc(dpy, event_base_return, error_base_return);
  fprintf(stderr, "XDamageQueryExtension returned %d\n", ret);
  fprintf(stderr, "But we will return 0\n");
  return 0;
}


}