/********************************************************************
*   File Name: mysocket.h
*   Description: 用于实现多客户端通信
*   Init Date: 2020/05/24
*********************************************************************/
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"       // 用于本地测试
// #define SERVER_IP "47.95.13.239"	// 用于公网测试
#define SERVER_PORT 18888
#define HANDSHARK_MSG "Hello,Client!\n"
#define MaxClientNum 10

/* 套接字信息结构体，用于记录客户端信息 */
typedef struct MySocketInfo{
    int socketCon;                  // 套接字描述符
    char *ipaddr;                   // 客户端IP地址
    uint16_t port;                  // 客户端端口号
}_MySocketInfo;

char buffer[BUFSIZ];                // 服务器数据收发缓冲区
int ReadToSend;                     // 服务器准备发送标志位

/* 用于记录客户端信息的数组 */
struct MySocketInfo arrConSocket[MaxClientNum];
int conClientCount;                 // 当前客户端数量

/* 用来与客户端通信的线程数组 */
pthread_t arrThrReceiveClient[MaxClientNum];
int thrReceiveClientCount;          // 当前通信子线程数量

/* 线程功能函数 */
void *fun_thrReceiveHandler(void *socketInfo);
void *fun_thrAcceptHandler(void *socketListen);
int checkThrIsKill(pthread_t thr);
