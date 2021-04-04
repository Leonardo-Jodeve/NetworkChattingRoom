/*
 * myhead.h
 *
 *  Created on: 2011-7-27
 *      Author: root
 */

#ifndef MYHEAD_H_
#define MYHEAD_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>
#include <sqlite3.h>

#define LOGIN 0                           /* 宏定义登录的动作为0 */
#define REGIST 1                          /* 宏定义注册的动作为1 */
#define CHATONE 2                         /* 宏定义对单人说话的动作为2 */
#define CHATALL 3                         /* 宏定义注册的动作为3 */
#define CHECK 4                           /* 宏定义查找的动作为4 */
#define HELP 5                            /* 宏定义帮助文档的动作为5 */
#define EXIT 6                            /* 宏定义退出聊天室的动作为6 */
#define EXPRESSION 7                      /* 宏定义表情符号的动作为7 */
#define PHRASE 8                          /* 定义短语的动作为8 */
#define SLIENCE 9                         /* 定义禁言的动作为9 */
#define TICK 10                           /* 定义替人的动作10 */
#define PERMITE 11                        /* 定义解禁的动作为11 */

#define TRANS_FILENAME 12                 /* 定义传输文件名 */
#define TRANS_FILEING 13                  /* 定义传输文件内容 */
#define TRANS_FILE_ACCEPT 14              /* 定义同意接收文件 */
#define TRANS_FILE_REFUSE 15              /* 定义拒绝接收文件 */
#define TRANS_FILE_FINISH 16              /* 定义传输文件完毕 */
#define WRONG_CMD -1                      /* 宏定义错误命令的动作为 -1 */

#define BACK 1                            /* 定义服务器端回传提示信息为1 */
#define CHAT 2                            /* 定义服务器端回传聊天内容为2 */

#define WRONG_CMD -1                      /* 宏定义错误命令的动作为 -1 */

#define port 8000                         /* 宏定义端口号为 8000 */

/* 定义一个结构体，用来封装消息 */
typedef struct _Message
{
    int action; /* 动作，具体的值已在宏中定义 */
    char message[1000]; /* 存放聊天的内容 */
    char user[20]; /* 存放用户名 */
    char target[20]; /* 存放聊天的目标，具体为某个在线的用户名或all */
    char password[20]; /* 存放密码 */
} Message;

/* 定义服务器端回传信息数据包 */
typedef struct _Back
{
    int type; /* 定义服务器回传数据的类型 */
    char user[20]; /* 定义回传的用户名 */
    char message[1024]; /* 存放回传的信息的内容 */
} Back_Msg;

int sockfd; /*定义一个全局变量来保存套接字 */
pthread_mutex_t mutex; /*定义一个互斥锁在多线程中使用 */
int fd_write; /* 传输文件需要的文件描述符 */
int fd_read; /* 传输文件需要的文件描述符 */
char buf_all[20]; /* 多线程需要用到的全局变量缓冲区 */


#endif /* MYHEAD_H_ */
