#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    pid_t pid;

    int pipe_fd[2];

    char buffer[1024];
    char data[] = "Pipe test program";

    int readCount, writeCount;

    memset(buffer, 0, sizeof(buffer));

    if(pipe(pipe_fd) < 0)
    {
        perror("Creat pipe error!\n");
        exit(1);
    }

    pid = fork();

    if(pid < 0)
    {
        perror("Create child thread failed!\n");
        exit(1);
    }
    else if(pid == 0)
    {
        close(pipe_fd[1]);
        sleep(3);

        if((readCount = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
        {
            printf("Read from pipe: %s", buffer);
        }
        
        close(pipe_fd[0]);
        exit(0);
    }
    else if(pid > 0)
    {
        close(pipe_fd[0]);
        sleep(1);

        if((writeCount = write(pipe_fd[1], data, sizeof(data))) != -1)
        {
            printf("Write to pipe: %s", data);
        }
        
        close(pipe_fd[1]);
        exit(0);
    }

    return 0;
}