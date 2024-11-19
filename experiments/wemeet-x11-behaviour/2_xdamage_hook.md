# 2. xdamage欺骗hook

上一个例子中我们发现xdamage的正常运作会使得腾讯会议仅调用一次`XShmGetImage`. 因此，这个版本的hook在之前的基础上，劫持`XDamageQueryExtension`函数，欺骗腾讯会议`XDamge`拓展不可用.

将`lib2_xdamage_hook.so`挂在腾讯会议上，发起会议并开启屏幕共享，此时stderr上的log则变为如下：


```bash

wemeetapp called XShmAttach(0x78b3c8003800, 0x78b3c8022d90)
now we are printing the info about this xdisplay
dpy->fd: 211
dpy->proto_major_version: 11
dpy->proto_minor_version: 0
dpy->vendor: The X.Org Foundation
dpy->qlen: 0
dpy->last_request_read: 10
dpy->request: 10
dpy->max_request_size: 65535
dpy->display_name: :0
dpy->default_screen: 0
dpy->nscreens: 1
dpy->motion_buffer: 256
dpy->min_keycode: 8
dpy->max_keycode: 255
dpy->xdefaults: Xcursor.size:   56
Xcursor.theme:  Notwaita-Black
Xft.antialias:  1
Xft.dpi:        168
Xft.hinting:    1
Xft.hintstyle:  hintslight
Xft.rgba:       rgb

dpy->screens: 0x78b3c8003120
and now we are going to print some more info about shminfo
shminfo->shmid: 40
shminfo->shmaddr: 0x78b3a8438000
shminfo->readOnly: 0
XDamageQueryExtension(0x78b3c8003800, 0x78b3c8001088, 0x78b3c800108c)
XDamageQueryExtension returned 1
But we will return 0
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
image->width: 2880
image->height: 1800
image->xoffset: 0
image->format: 2 # indicates ZPixmap
image->data: 0x78b3a8438000
image->byte_order: 0
image->bitmap_unit: 32
image->bitmap_bit_order: 0
image->bitmap_pad: 32
image->depth: 24
image->bytes_per_line: 11520
image->bits_per_pixel: 32 # 4 bytes per pixel, ZPixmap, thus RGBA or BGRA
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)
wemeetapp called XShmGetImage(0x78b3c8003800, 1089, 0x78b3c80136d0, 295, 2016, 18446744073709551615)

...

wemeetapp called XShmDetach(0x78b3c8003800, 0x78b3c8022d90)

```

如上，可以看到如同我们的预期，此时腾讯会议会通过反复调用`XShmGetImage`来获取屏幕内容的每一帧. 我们最终的hook就是要利用这个机制，把正确的图像放入`XshmGetImage`返回的`XImage`结构体中，从而实现录屏.