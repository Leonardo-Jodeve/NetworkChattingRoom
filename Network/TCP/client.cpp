#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 32768

void * receiveMessage(void* arg)
{
    int serverFD = *((int *)arg);
    int receivedLength;

    char buffer[1024];

    while (1)
    {
        receivedLength = recv(serverFD, buffer, sizeof(buffer), 0);
        if(receivedLength == 0)
        {
            printf("Server has been shutdown!\n");
            pthread_exit(NULL);
        }
        printf("Received from server: %s", buffer);
    }
    pthread_exit(NULL);  
}

int main(int argc, char* argv[])
{
    int serverFD;

    struct sockaddr_in serverAddress;

    pthread_t id;

    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFD < 0)
    {
        perror("Socket create error!");
        exit(1);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.61.130");

    if(connect(serverFD, (struct sockaddr*)(&serverAddress), sizeof(struct sockaddr_in)) < 0)
    {
        perror("Connect error!");
        exit(1);
    }
    printf("Connect success!\n");

    pthread_create(&id, NULL, receiveMessage, (void *)(&serverFD));

    while(1)
    {
        char buffer[1024];

        memset(buffer, 0, sizeof(buffer));
        printf("Please input message sent to the server:");
        scanf("%s", buffer);
        send(serverFD, buffer, strlen(buffer), 0);
        usleep(3);

    }

    // if(pthread_create(&id, NULL, sendMessage, (void *)(&socketFD)) != 0)
    // {
    //     perror("Pthread create error!");
    //     exit(1);
    // }
    
    // send(socketFD, "Hello World!\n",13, 0);

    // char buffer[1024];
    // recv(socketFD, buffer, sizeof(buffer),0);

    // printf("Received from server: %s", buffer);

    shutdown(serverFD, SHUT_RDWR);

    return 0;
}