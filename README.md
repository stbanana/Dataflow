# Dataflow
Dataflow middleware. Enhanced use of DMA and half-duplex.

# 作用
期望帮助那些管理不好传输流的 DMA 以及半双工用法的小白。
一个典型的应用场景是，串口的 RS485 通信。

# 副作用
使用本库会带来 1~2ms 的额外时间开销，想要尽可能跑满信道还是自己编写驱动吧。

但我相信对于小白而言，这不算什么。
