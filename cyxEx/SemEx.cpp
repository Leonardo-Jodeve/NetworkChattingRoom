#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
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
        printf("Child thread pid is %d\n", getpid());
        semV(semID);
    }
    if(pid > 0)
    {
        semP(semID);
        printf("Father thread pid is %d\n", getpid());
        semV(semID);
        delSem(semID);
    }
    return 0;
}