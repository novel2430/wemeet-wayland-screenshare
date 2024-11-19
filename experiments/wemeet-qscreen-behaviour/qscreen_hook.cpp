
#include <cstdio>
#include <dlfcn.h>
#include <QScreen>
#include <QWindow>
#include <QPixmap>
#include <QDebug>

auto dl_handle = dlopen("libQt5Gui.so", RTLD_LAZY);
typedef QPixmap (*GrabWindowFunc)(QScreen* self, WId window, int x, int y, int w, int h);
GrabWindowFunc originalFunction = (GrabWindowFunc)dlsym(dl_handle, "_ZN7QPixmap10grabWindowEyiiii");

QPixmap QScreen::grabWindow(WId window, int x, int y, int w, int h)
{
  fprintf(stderr, "This pointer: %p\n", this);
  fprintf(stderr, "QScreen::grabWindow hooked\n");
  fprintf(stderr, "window: %llu, x: %d, y: %d, w: %d, h: %d\n", window, x, y, w, h);
  fprintf(stderr, "original function: %p\n", originalFunction);


  QScreen* screen = this;
  printf("Screen: %p\n", screen);
  printf("name: %s\n", screen->name().toStdString().c_str());
  printf("manufacturer: %s\n", screen->manufacturer().toStdString().c_str());
  printf("model: %s\n", screen->model().toStdString().c_str());
  printf("serialNumber: %s\n", screen->serialNumber().toStdString().c_str());
  printf("depth: %d\n", screen->depth());
  printf("size: %d x %d\n", screen->size().width(), screen->size().height());
  printf("geometry: %d x %d\n", screen->geometry().width(), screen->geometry().height());
  

  return originalFunction(this, window, x, y, w, h);
}