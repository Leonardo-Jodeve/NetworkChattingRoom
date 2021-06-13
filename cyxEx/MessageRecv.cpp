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

    do
    {
        memset(msg.msg_text, 0, sizeof(msg.msg_text));

        if((msgrcv(qid, &msg, sizeof(msg.msg_text), 0, 0)) < 0)
        {
            perror("Message send error!\n");
            exit(1);
        }

        printf("The message from %ld is %s\n", msg.msg_type, msg.msg_text);

        if(strncmp(msg.msg_text, "quit", 4) == 0)
        {
            break;
        }
    }while(strncmp(msg.msg_text, "quit", 4) != 0);

    if((msgctl(qid, IPC_RMID, NULL)) < 0)
    {
        perror("msgctl error!\n");
        exit(1);
    }

    exit(0);

    return 0;
}