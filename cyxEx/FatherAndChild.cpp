#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    pid_t pid;

    pid = fork();

    if(pid > 0)
    {
        printf("I am father thread, my child's pid is %d\n", pid);
    }
    if(pid == 0)
    {
        printf("I am child thread\n");
    }

    return 0;
}