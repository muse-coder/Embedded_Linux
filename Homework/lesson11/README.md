## 说明文档

> Linux C 编程： 使用命名管道FIFO实现客户端服务器的通信

### 文件概述

| 文件名 | 功能 |
| :----------: | :-----------: |
| server.c | 服务端的源文件 |
| client.c | 客户端的源文件 |
| fifo.c | 功能接口函数定义 |
| fifo.h | 全局变量和功能函数声明 |
| Makefile | 工程编译规则 |

### 代码流程简介

> 对于服务端而言

1. 首先使用make工具编译生成客户端client和服务端server
2. 运行服务端，服务端首先注册SIGINT的信号处理函数，用于实现服务端退出功能
3. 服务端创建公有命名管道，并以只读方式打开管道文件
4. 读取来自客户端的结构体数据信息
5. 如果客户端的数据被标识为IS_NEW_CLIENT，说明该客户端为初次通信，服务端会为此客户端创建相应的私有命名管道，该命名管道以次客户端的PID为标识
6. 接下来服务端继续判断客户端的消息，如果消息是CLIENT_QUIT，表示客户端已退出，此时服务端需要关闭该条私有管道链接。
7. 如果客户端的数据为正常的消息，服务端会从私有管道回复客户端

> 对于客户端而言

1. 运行客户端程序，客户端首先会将自己的标识信息PID通过共有管道发送给服务端
2. 服务端会为新的客户端创建相应的私有管道
3. 接下来客户端只需读取相应的私有管道中的数据即可

### 关键问题及解决方式

* 问题一、如何判断服务器接收到的数据来自于哪个客户端
1. 这实际上就是要实现对客户端的标识，一种方式是在客户端传输的数据前后，加上报头报尾将数据封装成成块的信息，类似于TCP协议的层层封装；第二种方式是客户端每次传输的数据都做成结构体的形式，结构体中包含用于表示客户端标识的成员变量，服务端读取该成员变量的值，即可判断出是哪个客户端。
2. 第一种方式 ~~应该需要对数据进行字符串合并拆分等操作，可能比较麻烦~~ 可以利用strncmp函数判断报头数据，~~所以~~本程序选择了使用第二种方式。**定义一个结构体类型，其中的一个结构体成员为客户端的PID标识，另一个成员则是客户端实际要发送的数据。**
3. 在每个客户端启动后，都会首先将自己的PID标识连同IS_NEW_CLIENT信息发送给服务器，由服务器生成私有管道链路，相当于客户端去给自己做了通信链路的注册。由于私有管道文件名的生成规则已经在客户端和服务器之间制定了协议，因此**服务端不必记住所有已注册的客户端，只需定向发送即可。**

* 问题二、如何实现客户端和服务器的退出
1. 对于每个客户端来说，向服务端发送CLIENT_QUIT标识的数据之后，客户端就直接结束进程。
2. 对于服务器来说，当收到来自客户端的CLIENT_QUIT数据之后，就将相应客户端的私有管道断开，删除私有管道文件。
3. 服务端启动后不能自行结束，需要按下CTRL+C产生SIGINT信号。服务端捕获到SIGINT信号后，将会断开共有管道，删除共有管道文件。

### 程序流程

![lesson11_flow](../../Image/Homework/lesson11/lesson11_flow.png)

### 代码构建方法

```c
// In the project root directory
make            /* Build client and server */
make clean      /* Clear object files and executable files */
```

### 调试过程演示

> 下面是发布在B站的演示视频，您也可以点击链接以查看部署在阿里云上的[原视频](http://47.95.13.239/Study/Linux/image/lesson11_debug.mp4)

[BiliBili - 命名管道_客户端服务器模型_测试视频](https://www.bilibili.com/video/BV1ne41147xk/)

### 致谢

感谢 @葛晟宏 @王文州 同学对代码提出的修改意见

### 开源协议

* 本项目遵循 [GPL](https://github.com/ZHJ0125/Embedded_Linux/blob/master/LICENSE) 开源许可协议
* GitHub 仓库地址 [https://github.com/ZHJ0125/Embedded_Linux/tree/master/Homework/lesson11](https://github.com/ZHJ0125/Embedded_Linux/tree/master/Homework/lesson11)
* Gitee 仓库地址 [https://gitee.com/zhj0125/Embedded_Linux/tree/master/Homework/lesson11](https://gitee.com/zhj0125/Embedded_Linux/tree/master/Homework/lesson11)
