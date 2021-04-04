#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <pthread.h>

#define PORT 32768

void * receiveMessage(void * arg)
{
    int clientFD = *((int *)arg);
    int receivedCount;
    char buffer[1024];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        if((receivedCount = recv(clientFD, buffer, sizeof(buffer), 0)) < 0)
        {
            perror("Receive error!");
            exit(1);
        }
        if(receivedCount == 0)
        {
            printf("Client %d has been shutdown\n", clientFD);
            pthread_exit(NULL);
        }
        printf("Received: %s\n", buffer);
        usleep(3);
    }
    
    pthread_exit(NULL);
}

void * sendMessage(void * arg)
{
    int clientFD = *((int *)arg);
    int sendLength;

    while(1)
    {
        sendLength = send(clientFD, "Hello World!\n", 14, 0);
        if(sendLength == 0)
        {
            printf("Client has been shutdown!\n");
            pthread_exit(NULL);
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    int socketFD;
    int clientFD;

    int clientAddressLength;

    struct sockaddr_in socketAddress;
    struct sockaddr_in clientAddress;

    char buffer[1024];

    pthread_t id;
    
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD == -1)
    {
        perror("socket create error!");
        exit(1);
    }
    printf("socket creat success!\n");

    bzero(&socketAddress, sizeof(struct sockaddr_in));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(PORT);
    socketAddress.sin_addr.s_addr = inet_addr("192.168.61.130");

    int opt = 1;

    setsockopt(socketFD, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    
    if(bind(socketFD, (struct sockaddr*)&socketAddress, sizeof(struct sockaddr_in)) < 0)
    {
        perror("Bind error!");
        exit(1);
    }
    printf("Bind success!\n");

    if(listen(socketFD, 3) < 0)
    {
        perror("Listen error!");
        exit(1);
    }
    printf("Listen success!\n");

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        
        bzero(&clientAddress, sizeof(struct sockaddr_in));
    
        clientAddressLength = sizeof(struct sockaddr_in);
        printf("Accepting.......\n");
        
        clientFD = accept(socketFD, (struct sockaddr*)(&clientAddress), (socklen_t *)(&clientAddressLength));

        if(clientFD < 0)
        {
            perror("Accept error!");
            exit(1);
        }
        printf("Accept success!\n");

        if(pthread_create(&id, NULL, receiveMessage, (void *)(&clientFD)) != 0)
        {
            perror("Pthread receive create error!");
            exit(1);
        }
        if(pthread_create(&id, NULL, sendMessage, (void *)(&clientFD)) != 0)
        {
            perror("Pthread send create error!");
            exit(1);
        }

        usleep(3);
        // printf("Port = %d Address = %s\n", ntohs(clientAddress.sin_port), inet_ntoa(clientAddress.sin_addr));

        // recv(clientFD, buffer, sizeof(buffer), 0);
        // printf("Received: %s", buffer);
        // usleep(2);
        // send(clientFD, buffer, strlen(buffer),0);

        // shutdown(clientFD, SHUT_RDWR);
    }
    

    return 0;
}