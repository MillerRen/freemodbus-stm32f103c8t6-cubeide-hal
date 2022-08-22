# freemodbus-stm32f103c8t6-cubeide-hal
freemodbus在stm32f103c8t6上的移植，使用cubeide和hal实现

## 使用方法
使用cubeide，可以直接打开项目文件即可

## 移植步骤参考
1. 下载freemodbus源码
1. 拷贝freemodbus里面的modbus目录里面所有文件到你的项目文件，这部分是框架，不需要动，作为库依赖就行。
1. 拷贝freemodbus的demo目录下的bare目录，这部分是需要移植的部分。
1. freemodbus需要使用串口进行通信，并且使用了定时器隔离modbus读写（至少3.5字节时间），因此需要在相应硬件上实现这两部分功能移植。主要包括初始化、开启和关闭串口，通过串口接收和发送数据，串口中断时通知freemodbus进行处理；初始化、开启和关闭定时器，定时器中断时，通知freemodbus进行处理。
1. 实现modbus的对应功能回调函数，例如读写线圈，输入寄存器，保持寄存器等回调函数。
