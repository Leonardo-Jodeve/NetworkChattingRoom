#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    int fd;
    char log[] = "This is a log.\n";

    pid = fork();
    if(pid < 0)
    {
        perror("Fork error!\n");
        exit(1);
    }
    else if(pid > 0)
    {
        exit(0);
    }

    setsid();

    chdir("/");

    umask(0);

    for(int i = 0; i < getdtablesize(); i++)
    {
        close(i);
    }

    for(int i = 0; i < 20; i++)
    {
        if((fd = open("/home/jodeve/Desktop/VS/cyxEx/log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0)
        {
            perror("Open file error!\n");
            exit(1);
        }
        write(fd, log, sizeof(log));
        close(fd);
        usleep(500);
    }
    exit(0);
    return 0;
}