#include <cstdio>
#include "original_funcs.hpp"


extern "C"
{
  
// we first hookup XShmAttach
// this is expected to be called when you start screencast
Bool XShmAttach(Display* dpy, XShmSegmentInfo* shminfo)
{
  fprintf(stderr, "wemeetapp called XShmAttach(%p, %p)\n", dpy, shminfo);
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
  Bool ret = XShmGetImageFunc(dpy, d, image, x, y, plane_mask);
  return ret;
}


}