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


struct receiveMessageFromClient
{
    int command;
    /**
     * command list:
     * 0. regist
     * 1. login
     * 2. send message to somebody
     * 3. send message to everyone who are online
     * etc;
    */
    char username[32];
    char passwordHash[256];

    int token;

    char sendToName[32];
    char message[512];
};

struct sendMessageToClient
{
    int command;
    int result;
    /**
     * result list:
     * 0: all right
     * -1: error
    */
    
    char receiveFromName[32];
    char message[512];
};

struct onlineUser
{
    int clientFD;
    char username[32];
    struct onlineUser *next;
};

struct onlineUser *head = NULL;

void addOnlineUser(struct onlineUser *newOnline)
{
    if(head == NULL)
    {
        newOnline->next == NULL;
        head = newOnline;
    }
    else
    {
        newOnline->next = head->next;
        head->next = newOnline;
    }
}

int findOnlineUser(char* username)
{
    struct onlineUser *temp;
    temp = head;
    while(temp != NULL)
    {
        if(strcmp(username, temp->username) == 0)
        {
            return temp->clientFD;
        }
    }
    return -1;
}

void * receiveMessage(void * arg)
{
    printf("Init success\n");
    int clientFD = *((int *)arg);
    int receivedCount;
    
    struct receiveMessageFromClient *receivedMessage;
    receivedMessage = (struct receiveMessageFromClient *)malloc(sizeof(struct receiveMessageFromClient));
    bzero(receivedMessage, sizeof(struct receiveMessageFromClient));


    while (1)
    {
        if((receivedCount = recv(clientFD, receivedMessage, sizeof(struct receiveMessageFromClient), 0)) < 0)
        {
            perror("Receive error!");
            exit(1);
        }
        if(receivedCount == 0)
        {
            printf("Client %d has been shutdown\n", clientFD);
            pthread_exit(NULL);
        }
        /**
     * command list:
     * 0. regist
     * 1. login
     * 2. send message to somebody
     * 3. send message to everyone who are online
     * etc;
    */
        switch (receivedMessage->command)
        {
        case 0:
            
            break;
        case 1:
            printf("Received login request!\n");
            struct onlineUser *newUser;
            newUser = (struct onlineUser *)malloc(sizeof(struct onlineUser));
            newUser->clientFD = clientFD;
            strcpy(newUser->username,receivedMessage->username);
            addOnlineUser(newUser);
            struct sendMessageToClient *message;
            message = (struct sendMessageToClient *)malloc(sizeof(struct sendMessageToClient));
            message->command = 1;
            message->result = 0;
            send(clientFD, message, sizeof(struct sendMessageToClient), 0);
            break;
        }

        usleep(3);
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

        pthread_create(&id, NULL, receiveMessage, (void *)&clientFD);

        usleep(3);
        
    }
    

    return 0;
}