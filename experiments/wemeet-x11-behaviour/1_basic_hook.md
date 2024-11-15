# 1. 基本的hook


将`lib1_basic_hook.so`挂到wemeetapp后，开始一个快速会议，并开启屏幕共享，我们会在stderr看到如下信息：

```bash

wemeetapp called XShmAttach(0x714af0003800, 0x714af0022d20)
wemeetapp called XShmGetImage(0x714af0003800, 1089, 0x714af00136d0, 0, 0, 18446744073709551615)
wemeetapp called XShmDetach(0x714af0003800, 0x714af0022d20)

```

可以看到，按照我们预期地，wemeetapp绕过操作系统上层接口，非常hacky地直接和X11 server通信，而这也正是它不可能在wayland上正常运作的原因。

> do you think mofo tencent's ever gonna fix this? Don't be stupid my son.

对于上述log，更为具体地：

1. 我们会注意到，在我们点击屏幕共享按钮，并随后选择了一个屏幕进行共享**后的一瞬间**，`XShmAttach`函数被调用了

2. 我们同时也会看到，`XShmGetImage`紧接着被调用了一次，但随后我们的stderr就没有更新的消息被打印出来了

3. 直到我们点击结束共享，`XShmDetach`才被调用

这里的主要问题是，为什么`XShmGetImage`只被调用了一次？在录屏时，难道wemeetapp不需要反复获取屏幕内容吗？问题的答案是：

- wemeetapp使用了XDamage拓展. 腾讯的开发者编写的逻辑是，利用XDamage，只抓取被录制的屏幕中被更新的区域. 这不得不说是一个很好的设计.
- 然而，在Xwayland环境下这套逻辑并没法work. wemeetapp在Xwayland上从XDamage中获取到的信息永远都在告诉他没有任何屏幕内容存在更新. 也因此，wemeetapp从头至尾只调用了一次`XShmGetImage`，用来获取那最初的一帧图像.
- 因此，我们当前版本的hook就是直接把XDamage的逻辑破坏掉，强迫wemeetapp去反复获取完整的帧图像.
