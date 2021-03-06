#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct message
{
    long msg_type;
    char msg_text[1024];
};

int main(int argc, char* argv[])
{
    int qid;
    key_t key;
    struct message msg;

    if((key = ftok(".", 'a')) == -1)
    {
        perror("Ftok error!\n");
        exit(1);
    }

    if((qid = msgget(key, IPC_CREAT|0666)) == -1)
    {
        perror("msgget error!\n");
        exit(1);
    }

    printf("Open queue %d success!\n", qid);

    while (1)
    {
        printf("Input some message:");
        scanf("%s", msg.msg_text);

        msg.msg_type = getpid();

        if((msgsnd(qid, &msg, strlen(msg.msg_text), 0)) < 0)
        {
            perror("Message send error!\n");
            exit(1);
        }

        if(strncmp(msg.msg_text, "quit", 4) == 0)
        {
            break;
        }
    }
    exit(0);

    return 0;
}