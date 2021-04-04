/*
 * check_putin.c
 *
 *  Created on: 2011-7-27
 *      Author: 
 */ 

#include "../../include/myhead.h"

/*函数作用：字符串解析函数，不同的命令返回不同的值
 *输入：字符串指针buff
 *输出：无
 *返回值：动作宏
 */
int check_putin(char *cmd)
{
    char check[10];
    int i = 0;

    /* 首先检查一下是不是普通命令 */
    for (i = 0; i < 3; i++)
    {
        check[i] = cmd[i];
    }
    check[i] = '\0';

    /* 当命令为登录时 */
    if (strcmp(check, "log") == 0)
    {
        return LOGIN;
    }

    /* 当命令为注册时 */
    if (strcmp(check, "reg") == 0)
    {
        return REGIST;
    }

    /* 当命令为对单人说话时 */
    if (strcmp(check, "sto") == 0)
    {
        return CHATONE;
    }

    /*  当命令为对所有人说话时 */
    if (strcmp(check, "sta") == 0)
    {
        return CHATALL;
    }

    /* 当命令为查看当前在线人数时 */
    if (strcmp(check, "che") == 0)
    {
        return CHECK;
    }

    /* 当命令为查看帮助时 */
    if (strcmp(check, "hlp") == 0)
    {
        return HELP;
    }

    /* 当命令为退出时 */
    if (strcmp(check, "ext") == 0)
    {
        return EXIT;
    }

    /* 当命令为禁言时 */
    if (strcmp(check, "nos") == 0)
    {
        return SLIENCE;
    }

    /* 当命令为解禁时 */
    if (strcmp(check, "say") == 0)
    {
        return PERMITE;
    }

    /* 当命令为踢人时 */
    if (strcmp(check, "out") == 0)
    {
        return TICK;
    }

    /* 命令为传输文件时 */
    if (strcmp(check, "tra") == 0)
    {
        return TRANS_FILENAME;
    }
    if (strcmp(check, "snd") == 0)
    {
        return TRANS_FILEING;
    }

    /* 命令为同意接收文件 */
    if (strcmp(check, "yes") == 0)
    {
        return TRANS_FILE_ACCEPT;
    }

    /* 命令为不同意接收文件 */
    check[2] = '\0';
    if (strcmp(check, "no") == 0)
    {
        return TRANS_FILE_REFUSE;
        close(fd_write);
    }

    /* 比较第一个输入的字符，判断是不是发送表情或短语 */
    check[1] = '\0';
    if (strcmp(check, ":") == 0)
    {
        return EXPRESSION;
    }
    if (strcmp(check, "/") == 0)
    {
        return PHRASE;
    }

    return WRONG_CMD; /*以上命令都不是的话就是错误命令 */
}


