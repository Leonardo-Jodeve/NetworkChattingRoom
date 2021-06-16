#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

extern int initSem(int semID, int initValue);

extern int delSem(int semID);

extern int semP(int semID);

extern int semV(int semID);

int main(int argc, char* argv[])
{
    pid_t pid;
    int semID;

    semID = semget(ftok(".",'a'), 1, 0666|IPC_CREAT);
    initSem(semID, 0);

    pid = fork();

    if(pid < 0)
    {
        perror("frok error!\n");
        exit(1);
    }
    else if(pid == 0)
    {
        printf("Child thread initializing...\n");
        sleep(3);
        printf("Child thread initialized, pid is %d\n", getpid());
        semV(semID);
        raise(SIGSTOP);
    }
    if(pid > 0)
    {
        semP(semID);
        
        printf("Father is trying to kill child thread %d\n", pid);

        if((waitpid(pid, NULL, WNOHANG)) == 0)
        {
            if(kill(pid, SIGKILL) == 0)
            {
                printf("Father killed %d\n", pid);
            }
        }
        
        waitpid(pid, NULL, 0);

        semV(semID);
        delSem(semID);
    }
    return 0;
}