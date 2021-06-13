#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int initSem(int semID, int initValue)
{
    union semun sem_union;
    sem_union.val = initValue;

    if(semctl(semID, 0, SETVAL, sem_union) == -1)
    {
        perror("Init sem error!\n");
        return -1;
    }
    return 0;
}

int delSem(int semID)
{
    union semun sem_union;
    if(semctl(semID, 0, IPC_RMID, sem_union) == -1)
    {
        perror("Del sem error!\n");
        return -1;
    }
    return 0;
}

int semP(int semID)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;

    if(semop(semID, &sem_b, 1) == -1)
    {
        perror("P operation error!\n");
        return -1;
    }
    return 0;
}

int semV(int semID)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;

    if(semop(semID, &sem_b, 1) == -1)
    {
        perror("V operation error!\n");
        return -1;
    }
    return 0;
}