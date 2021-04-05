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

bool isLogin = false;

struct sendMessageToServer
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

    char sendToName[32];
    char message[512];
};

struct receiveMessageFromServer
{
    int command;
    /**
     * command list:
     * 0. regist
     * 1. login
     * 2. receive message from somebody
     * 3. receive message from broadcast
     * etc;
    */
    int result;
    /**
     * result list:
     * 0: all right
     * -1: error
    */
    
    char receiveFromName[32];
    char message[512];
};

void * receiveMessage(void* arg)
{
    int serverFD = *((int*)arg);
    int receivedLength;
    struct receiveMessageFromServer *receivedMessage;
    receivedMessage = (struct receiveMessageFromServer *)malloc(sizeof(struct receiveMessageFromServer));
    bzero(receivedMessage, sizeof(struct receiveMessageFromServer));

    while (1)
    {
        receivedLength = recv(serverFD, receivedMessage, sizeof(struct receiveMessageFromServer), 0);
        if(receivedLength == 0)
        {
            printf("Server has been shutdown!\n");
        }

        if(receivedMessage->result == 0)
        {
            switch (receivedMessage->command)
            {
            case 0:
                printf("Regist success! Please login.\n");
                break;
                
            
            case 1:
                printf("Login success!\n");
                isLogin = true;


                
                break;
            case 2:
                printf("%s send you a message:%s", receivedMessage->receiveFromName, receivedMessage->message);
                break;
            case 3:
                printf("%s broadcast a message:%s", receivedMessage->receiveFromName, receivedMessage->message);\
                break;
            }
        }
        else
        {
            switch (receivedMessage->command)
            {
            case 0:
                printf("Regist failed\n");
                break;
            
            case 1:
                printf("Login falied\n");
                isLogin = false;
                break;
            }
        }

    }
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

/**
     * command list:
     * 0. regist
     * 1. login
     * 2. receive message from somebody
     * 3. receive message from broadcast
     * etc;
    */
    struct sendMessageToServer *sendMessage;
    sendMessage = (struct sendMessageToServer *)malloc(sizeof(struct sendMessageToServer));

    while(! isLogin)
    {
        int choice;
        char username[32];
        char passwordHash[256];

        printf("0 to regist, 1 to login\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 0:
            
            bzero(sendMessage, sizeof(struct sendMessageToServer));

            printf("Please set a username:");
            scanf("%s", username);
            printf("Please set a password:");
            scanf("%s", passwordHash);

            sendMessage->command = 0;
            strcpy(sendMessage->username, username);
            strcpy(sendMessage->passwordHash, passwordHash);
            send(serverFD, sendMessage, sizeof(struct sendMessageToServer), 0);
            sleep(1);
            break;
        
        case 1:
            
            bzero(sendMessage, sizeof(struct sendMessageToServer));

            printf("Please enter username:");
            scanf("%s", username);
            printf("Please enter password:");
            scanf("%s", passwordHash);

            sendMessage->command = 1;
            strcpy(sendMessage->username, username);
            strcpy(sendMessage->passwordHash, passwordHash);
            send(serverFD, sendMessage, sizeof(struct sendMessageToServer), 0);
            sleep(1);
            break;

        default:
            printf("Error, please input correct choice!\n");
            continue;
        }

    }

    pthread_join(id, NULL);

    shutdown(serverFD, SHUT_RDWR);

    return 0;
}