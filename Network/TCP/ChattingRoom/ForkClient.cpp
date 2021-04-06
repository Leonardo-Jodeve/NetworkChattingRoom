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
char loggedinUsername[32];

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
    char password[256];

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
        bzero(receivedMessage, sizeof(struct receiveMessageFromServer));

        receivedLength = recv(serverFD, receivedMessage, sizeof(struct receiveMessageFromServer), 0);
        if(receivedLength == 0)
        {
            printf("Server has been shutdown!\n");
            pthread_exit(NULL);
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
                printf("%s send you a message:%s\n", receivedMessage->receiveFromName, receivedMessage->message);
                break;
            case 3:
                printf("%s broadcast a message:%s\n", receivedMessage->receiveFromName, receivedMessage->message);
                break;
            }
        }
        else
        {
            switch (receivedMessage->command)
            {
            case 0:
                printf("Regist failed\n");
                printf("%s", receivedMessage->message);
                break;
            
            case 1:
                printf("Login falied\n");
                isLogin = false;
                memset(loggedinUsername,0,sizeof(loggedinUsername));
                break;
            case 2:
                printf("Send private message failed!\n");
                break;
            case 3:
                printf("Send broadcast message failed!\n");
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

    while(! isLogin)
    {
        int choice = -1;
        char password[256];


        switch (choice)
        {
        case 0:
            struct sendMessageToServer *registMessage;
            registMessage = (struct sendMessageToServer *)malloc(sizeof(struct sendMessageToServer));
            bzero(registMessage, sizeof(struct sendMessageToServer));

            printf("Please set a username:");
            scanf("%s", loggedinUsername);
            printf("Please set a password:");
            scanf("%s", password);

            registMessage->command = 0;
            strcpy(registMessage->username, loggedinUsername);
            strcpy(registMessage->password, password);
            send(serverFD, registMessage, sizeof(struct sendMessageToServer), 0);
            sleep(1);
            break;
        
        case 1:
            struct sendMessageToServer *loginMessage;
            loginMessage = (struct sendMessageToServer *)malloc(sizeof(struct sendMessageToServer));
            bzero(loginMessage, sizeof(struct sendMessageToServer));

            printf("Please enter username:");
            scanf("%s", loggedinUsername);
            printf("Please enter password:");
            scanf("%s", password);

            loginMessage->command = 1;
            strcpy(loginMessage->username, loggedinUsername);
            strcpy(loginMessage->password, password);
            send(serverFD, loginMessage, sizeof(struct sendMessageToServer), 0);
            sleep(1);
            break;

        default:
            printf("Error, please input correct choice!\n");
            break;
        }

    }
    
    bool exit = false;
    while(!exit)
    {
        int choice = -1;
        printf("2 to send message, 3 to broadcast a message, 4 to exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 2:
            char personUsername[32];
            char privateMessage[512];

            printf("Please input username you want to send message to:");
            scanf("%s", personUsername);
            printf("Please input message:");
            scanf("%s", privateMessage);

            struct sendMessageToServer *personMessage;
            personMessage = (struct sendMessageToServer *)malloc(sizeof(struct sendMessageToServer));
            bzero(personMessage, sizeof(struct sendMessageToServer));

            personMessage->command = 2;
            strcpy(personMessage->message, privateMessage);
            strcpy(personMessage->sendToName, personUsername);
            strcpy(personMessage->username, loggedinUsername);

            printf("Message is supposed to send to %s\n", personUsername);
            printf("Message is sending to %s\n", personMessage->sendToName);

            send(serverFD, personMessage, sizeof(struct sendMessageToServer), 0);
            break;
        
        case 3:
            char broadcastMessage[512];

            printf("Please input broadcast message:");
            scanf("%s", broadcastMessage);

            struct sendMessageToServer *publicMessage;
            publicMessage = (struct sendMessageToServer *)malloc(sizeof(struct sendMessageToServer));
            bzero(publicMessage, sizeof(struct sendMessageToServer));

            publicMessage->command = 3;
            strcpy(publicMessage->message, broadcastMessage);
            strcpy(publicMessage->username, loggedinUsername);
            send(serverFD, publicMessage, sizeof(struct sendMessageToServer), 0);
            break;
        case 4:
            exit = true;
            pthread_cancel(id);
            break;
        default:
            {
                printf("Error, please input correct choice!\n");
                break;
            }
        }
    }

    shutdown(serverFD, SHUT_RDWR);

    return 0;
}