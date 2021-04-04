/*
 * pack_message.c
 *
 *  Created on: 2011-7-27
 *      Author: 
 */ 

#include "../../include/myhead.h"

/*函数作用：输出欢迎信息
 *输入：无
 *输出：介绍命令
 *返回值：无
 */
void login()
{
    printf(
            "*****************************************************************\n");
    printf("欢迎来到聊天室\n");
    printf("输入log登录\n");
    printf("输入reg注册\n");
    printf("输入sto对某人说话\n");
    printf("输入sta对大家说话\n");
    printf("输入che查看当前在线用户\n");
    printf("输入hlp查看帮助\n");
    printf("输入/用户/短语 发送短语   输入:)或:(发送表情\n");
    printf("支持的短语：/welcome,/smart,/funny\n");
    printf("输入tra传输文件（当前版本暫时只支持文本文件传输)\n");
    printf("超级用户命令：nos禁言，say解禁，out踢出在线用户\n");
    printf("输入ext退出聊天室\n");
    printf(
            "*****************************************************************\n");
}

/*函数作用：将数据打包
 *输入：动作action ,指向msg数据结构的指针*msg，输入的命令*cmd
 *输出：
 *返回值: 成功返回0
 */
int pack_message(int action, Message *msg, char *cmd)
{
    char buff[1024]; /* 缓冲区 */
    int i;
    int j;

    /* 根据action动作选项来选择下一步 */
    switch (action)
    {

    /* 当命令为登录时 */
    case (LOGIN):
    {
        while (1) /* 死循环，如果用户名过短或过长就重新读取 */
        {
            printf("请输入您的用户名,小于20个字节：\n");
            fgets(buff, 1024, stdin);
            for (i = 0; buff[i] != '\n'; i++)
            {
                msg->user[i] = buff[i];
            }
            if (strlen(buff) == 1)
            {
                printf("用户名为空！\n");
                continue;
            }
            if (i >= 20)
            {
                printf("用户名过长！\n");
                continue;
            }
            msg->user[i] = '\0';
            break;
        }
        while (1)/* 死循环，如果密码过短或过长就重新读取 */
        {
            printf("请输入您的密码,小于20个字节：\n");
            fgets(buff, 1024, stdin);
            for (i = 0; buff[i] != '\n'; i++)
            {
                msg->password[i] = buff[i];
            }
            if (strlen(buff) == 1)
            {
                printf("密码为空！\n");
                continue;
            }
            if (i >= 20)
            {
                printf("密码过长！\n");
                continue;
            }
            msg->password[i] = '\0';
            break;
        }
    }
        break;

        /* 当命令为注册时 */
    case (REGIST):
    {
        while (1)/* 循环，用户名过长或过短则重新读取 */
        {
            printf("请输入您的用户名,小于20个字节：\n");
            fgets(buff, 1024, stdin);
            for (i = 0; buff[i] != '\n'; i++)
            {
                msg->user[i] = buff[i];
            }
            if (i >= 20)
            {
                printf("用户名过长！\n");
                continue;
            }
            msg->user[i] = '\0';
            break;
        }
        char tmp[20];/* 临时字符串，用来比较两次的密码输入是否相同 */

        /* 循环体，检查两次密码输入是否相同 */
        while ((strcmp(msg->password, tmp) != 0)
                || (strlen(msg->password) == 0))
        {
            while (1) /* 循环，当密码过长或过短就重新读取 */
            {
                printf("请输入您的密码,小于20个字节：\n");
                fgets(buff, 1024, stdin);
                for (i = 0; buff[i] != '\n'; i++)
                {
                    msg->password[i] = buff[i];
                }
                if (i >= 20)
                {
                    printf("密码过长！\n");
                    continue;
                }
                msg->password[i] = '\0';
                strcpy(tmp, msg->password);
                printf("请再次输入密码，小于20字节：\n");
                fgets(buff, 1024, stdin);
                for (i = 0; buff[i] != '\n'; i++)
                {
                    msg->password[i] = buff[i];
                }
                if (i >= 20)
                {
                    printf("密码过长！\n");
                    continue;
                }
                msg->password[i] = '\0';
                if (strcmp(msg->password, tmp) != 0)
                {
                    printf("两次输入不一致！\n");
                }
                break;
            }
        }
    }
        break;

        /* 当命令为对单人聊天时 */
    case (CHATONE):
    {

        /* 把目标的用户名和聊天内容保存到msg数据包中 */
        printf("请输入您需要发送消息的对象：\n");
        fgets(buff, 1024, stdin);
        for (i = 0; buff[i] != '\n'; i++)
        {
            msg->target[i] = buff[i];
        }
        msg->target[i] = '\0';
        printf("请输入您需要发送的消息：\n");
        fgets(buff, 1024, stdin);
        for (i = 0; buff[i] != '\n'; i++)
        {
            msg->message[i] = buff[i];
        }
        msg->message[i] = '\0';
    }
        break;

        /* 当命令为对所有人聊天 */
    case (CHATALL):
    {
        strcpy(msg->target, "all");
        printf("请输入您需要发送的消息：\n");
        fgets(buff, 1024, stdin);
        for (i = 0; buff[i] != '\n'; i++)
        {
            msg->message[i] = buff[i];
        }
        msg->message[i] = '\0';
    }
        break;

        /* 当命令为传输文件请求时 */
    case (TRANS_FILENAME):
    {
        int i;
        int j;
        char buffer[1024];
        char tmp[1024];

        /* 取得需要发送的文件的对象 */
        printf("请输入您需要传输文件的对象：\n");
        fgets(buffer, 1024, stdin);
        for (i = 0; buffer[i] != '\n'; i++)
        {
            msg->target[i] = buffer[i];
        }
        msg->target[i] = '\0';

        /* 取得文件路径 */
        while (1)
        {
            bzero(buffer, 1024);
            printf("请输入需要传输的文件：(绝对路径)\n");
            fgets(buffer, 1024, stdin);
            bzero(tmp, 1024);
            for (i = 0; buffer[i] != '\n'; i++) /* 去掉换行符 */
            {
                tmp[i] = buffer[i];
            }
            tmp[i] = '\0';

            /* 检查文件路径是否正确 */
            fd_read = open(tmp, O_RDWR);
            if (fd_read < 0)
            {
                printf("不存在此文件,打开失败！\n");
                return 1;
            }
            break;
        }

        /* 取出文件名称 */
        j = 0;
        i = strlen(tmp) - 1;
        bzero(buffer, 1024);
        while (tmp[i] != '/')
        {
            i--;
        }
        for (i = i + 1; i <= strlen(tmp) - 1; i++)
        {
            buffer[j] = tmp[i];
            j++;
        }
        buffer[j] = '\0';
        strcpy(msg->message, buffer);
        msg->action = TRANS_FILENAME;
        printf("已向%s发送请求，等待回复\n", msg->target);
    }
        break;

        /* 开始发送文件 */
    case (TRANS_FILEING):
    {
        int n;
        while (1)
        {
            bzero(buff, strlen(buff));
            bzero(msg->message, strlen(msg->message));
            n = read(fd_read, buff, 900);
            buff[n] = '\0';

            /* 文件读取到末尾时动作信息改变为TRANS_FILE_FINISH,并跳出循环 */
            if (n == 0)
            {
                printf("发送完毕\n");
                msg->action = TRANS_FILE_FINISH;
                close(fd_read);
                break;
            }
            msg->action = TRANS_FILEING;
            strcpy(msg->message, buff);
            write(sockfd, msg, sizeof(*msg));
        }
    }
        break;

        /* 拒绝接收文件传输 */
    case (TRANS_FILE_REFUSE):
    {
        strcpy(msg->target, buf_all);
        printf("你拒绝了接收文件\n");
        strcpy(msg->message, msg->user);
        strcat(msg->message, "拒绝了接收文件"); /* 在数据包中写入拒绝信息 */
        close(fd_write);
    }
        break;

        /* 同意接收文件传输 */
    case (TRANS_FILE_ACCEPT):
    {
        strcpy(msg->target, buf_all);
        printf("你同意了接收文件\n");
    }
        break;

        /*  当命令为禁言时 */
    case (SLIENCE):

    {
        printf("请输入您需要禁言的对象：\n");
        fgets(buff, 1024, stdin);
        for (i = 0; buff[i] != '\n'; i++)
        {
            msg->target[i] = buff[i];
        }
        msg->target[i] = '\0';
    }
        break;

        /* 当命令为解禁时 */
    case (PERMITE):

    {
        printf("请输入您需要解禁的对象：\n");
        fgets(buff, 1024, stdin);
        for (i = 0; buff[i] != '\n'; i++)
        {
            msg->target[i] = buff[i];
        }
        msg->target[i] = '\0';
    }
        break;

        /* 当命令为踢人时 */
    case (TICK):

    {
        printf("请输入您需要踢出的对象：\n");
        fgets(buff, 1024, stdin);
        for (i = 0; buff[i] != '\n'; i++)
        {
            msg->target[i] = buff[i];
        }
        msg->target[i] = '\0';
    }
        break;

        /* 当命令为发送一个表情时 */
    case (EXPRESSION):
    {
        cmd[2] = '\0';

        /* 表情是笑脸 */
        if (strcmp(cmd, ":)") == 0)
        {
            strcpy(msg->message, "做了一个笑脸");
        }

        /* 表情是哭脸 */
        if (strcmp(cmd, ":(") == 0)
        {
            strcpy(msg->message, "做了一个哭脸");
        }
    }
        break;

        /* 当命令为发送短语时 */
    case (PHRASE):
    {
        for (i = 1; cmd[i] != '/'; i++) /* 将目标读取出来保存到msg中 */
        {
            msg->target[i - 1] = cmd[i];
        }
        msg->target[i - 1] = '\0';
        for (j = 0; cmd[i] != '\n'; i++) /* 将短语读取出来保存到缓冲区中 */
        {
            buff[j] = cmd[i];
            j++;
        }
        buff[j] = '\0';

        /* 根据不同的短语内容向msg->message中写于具体的内容 */
        if (strcmp(buff, "/welcome") == 0)
        {
            strcpy(msg->message, "大侠，欢迎来到咱们聊天室!");
        }
        else if (strcmp(buff, "/smart") == 0)
        {
            strcpy(msg->message, "你太有型了!");
        }
        else if (strcmp(buff, "/funny") == 0)
        {
            strcpy(msg->message, "你真搞笑！");
        }

        /* 不支持的短语则给出提示 */
        else
        {
            printf("错误的指令！\n");
            return 1;
        }
    }
        break;
    default:
        break;
    }
    return 0;
}


