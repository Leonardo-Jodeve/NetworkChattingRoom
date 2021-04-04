#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>

pthread_mutex_t mutex;



void* WriteHelloWorld(void * arg)
{
    int i = 0;
    char buffer[] = "HelloWorld";
    int fileFD = *(int*)arg;
    while(i < 5)
    {
        pthread_mutex_lock(&mutex);
        write(fileFD, buffer, 10);
        write(fileFD, "\n", 1);
        pthread_mutex_unlock(&mutex);
        sleep(1);
        i++;
    }
    close(fileFD);
    return (void *)0;
}

void* Writewwhhh(void * arg)
{
    int i = 0;
    char buffer[] = "wwhhh";
    int fileFD = *(int*)arg;
    while(i < 5)
    {
        pthread_mutex_lock(&mutex);
        write(fileFD, buffer, 5);
        write(fileFD, "\n", 1);
        pthread_mutex_unlock(&mutex);
        sleep(1);
        i++;
    }
    return (void *)0;
}



int main(int argc, char* argv[])
{
    pthread_t thread1;
    pthread_t thread2;

    int fileFD;
    fileFD = open("./text.txt", O_WRONLY | O_APPEND | O_CREAT,0777);

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread1, NULL, WriteHelloWorld, (void *)&fileFD);
    pthread_create(&thread2, NULL, Writewwhhh, (void *)&fileFD);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    close(fileFD);
    return 0;
}