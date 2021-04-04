/*
 * main.c
 *
 *  Created on: 2011-7-27
 *      Author: 
 */ 

#include "../../include/myhead.h" 

/* 建立一个线程来读取服务器发送来的信息 */
void read_msg()
{
    Back_Msg msg; /* 读数据的缓冲区 */
    char buffer[1024];
    while (1)
    {

        size_t n;
        if ((n = read(sockfd, &msg, sizeof(msg))) > 0) /* 读取服务器发来的数据并显示 */
        {
            switch (msg.type)
            {
            case (BACK): /* 当回传类型为提示信息时只显示内容 */
            {
                printf("%s\n", msg.message);
            }
                break;
            case (CHAT): /* 当内容为聊天记录时显示并保存 */
            {
                printf("%s\n", msg.message);
                FILE *fp;
                strcpy(buffer, msg.user);
                strcat(buffer, "_his");
                fp = fopen(buffer, "a+"); /* 聊天记录保存在用户名_his的文件中 */
                strcat(msg.message, "\n\n");
                fwrite(msg.message, strlen(msg.message), 1, fp);
                fclose(fp);
            }
                break;
            case (TRANS_FILENAME): /* 请求发送的文件名 */
            {

                /* 判断是否接受文件 */
                printf("%s要求发送%s,是否接收？（yes/no)\n请确认当前目录下没有同名文件\n", msg.user,
                        msg.message);
                strcpy(buffer, msg.message);
                strcpy(buf_all, msg.user);
                fd_write = open(buffer, O_CREAT|O_RDWR, 00777);
                if (fd_write < 0)
                {
                    printf("打开文件失败！\n");
                }
            }
                break;
            case (TRANS_FILEING): /* 内容为发送文件 */
            {
                write(fd_write, msg.message, strlen(msg.message));
            }
                break;
            case (TRANS_FILE_FINISH): /* 对方文件发送完毕 */
            {
                printf("文件接收完毕\n");
                close(fd_write);
                bzero(buf_all, strlen(buf_all));
            }
                break;
            case (TRANS_FILE_REFUSE): /* 内容为对方拒绝接收文件 */
            {
                printf("%s\n", msg.message);
                close(fd_read);
            }
                break;
            case (TRANS_FILE_ACCEPT): /* 对方同意接收文件，开始发送 */
            {
                printf("%s同意接收文件，请输入snd开始发送\n", msg.user);
            }
                break;
            default:
                break;
            }
        }

        else
        {
            printf("server out of line\n"); /*如果读不到数据则显示于服务器断开*/
            exit(1);
        }
    }
}

/* 主函数 */
int main(int argc, char *argv[])
{
    Message msg; /* 定义一个Message结构体 */
    char buff[1024]; /* 定义缓冲区 */
    struct sockaddr_in server_addr; /* 定义服务器的结构体 */
    struct hostent *host; /* 定义服务器地址 */
    int ret; /* 保存线程创建的返回值 */
    pthread_t id; /* 线程id */

    /* 查询服务器端的地址 */
    if (argc != 2)
    {
        fprintf(stderr, "Usage:%s hostname \a\n", argv[0]);
        exit(1);
    }
    if ((host = gethostbyname(argv[1])) == NULL)
    {
        fprintf(stderr, "Gethostname error\n");
        exit(1);
    }

    /* 调用socket函数创建一个TCP协议套接口 */
    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
        exit(1);
    }

    /* 填充服务器端的资料 */
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /* 连接服务器 */
    if (connect(sockfd, (struct sockaddr *) (&server_addr),
            sizeof(struct sockaddr)) == -1)
    {
        fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
        exit(1);
    }

    /* 互斥锁 */
    pthread_mutex_init(&mutex, NULL); /* 初始化互斥锁 */
    ret = pthread_create(&id, NULL, (void *) read_msg, NULL); /* 创建一个信线程来读取服务器端发送的信息 */
    if (ret != 0)
    {
        printf("Create pthread error!\n");
        exit(1);
    }

    login(); /* 打印欢迎信息 */
    /* 死循环，开始聊天 */
    while (1)
    {
        printf("请输入您的命令，输入hlp查看帮助：\n");
        bzero(buff, sizeof(buff)); /* 清空缓冲区 */
        fgets(buff, 1024, stdin); /* 将输入的数据存到buff中 */
        msg.action = check_putin(buff); /* 字符解析，返回到action中 */
        if (msg.action == WRONG_CMD) /* 当输入的为错误的命令时 */
        {
            printf("错误的指令\n");
            continue;
        }

        if (pack_message(msg.action, &msg, buff) == 1) /* 返回1时跳入下一次循环 */
        {
            continue;
        }

        if (msg.action == HELP) /* 当命令为帮助时，不将数据打包 */
        {
            login();
            continue;
        }
        if (msg.action == EXIT) /* 当命令为退出时，关闭进程 */
        {
            exit(0);
        }

        write(sockfd, &msg, sizeof(msg)); /* 发送数据包 */

        if (msg.action == REGIST) /* 注册结束后清空数据包 */
        {
            bzero(&msg.message, sizeof(msg.message));
            bzero(&msg.user, sizeof(msg.user));
            bzero(&msg.target, sizeof(msg.target));
            bzero(&msg.password, sizeof(msg.password));
        }
        sleep(1);
    }
    close(sockfd);
    exit(0);
}
