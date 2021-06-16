#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    pid_t pc, pr;

    pc = fork();
    if(pc < 0)
    {
        perror("Fork error!\n");
        exit(1);
    }
    else if(pc == 0)
    {
        sleep(5);
        exit(0);
    }
    else if(pc > 0)
    {
        do
        {
            pr = waitpid(pc, NULL, WNOHANG);
            if(pr == 0)
            {
                printf("The child process has not exit\n");
                sleep(1);
            }
        } while (pr == 0);
        
        if(pr == pc)
        {
            printf("Get child exit code:%d\n", pr);
        }
        else
        {
            printf("Some error occured.\n");
        }
    }
    return 0;
}