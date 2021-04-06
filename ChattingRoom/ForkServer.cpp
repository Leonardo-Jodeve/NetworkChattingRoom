#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <pthread.h>

#include <sqlite3.h>

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
    char password[256];

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

struct receiveFunctionArgs
{
    int clientFD;
    sqlite3* db;
};

struct onlineUser *head = (struct onlineUser *)malloc(sizeof(struct onlineUser));

void addOnlineUser(struct onlineUser *newOnline)
{
    newOnline->next = head->next;
    head->next = newOnline;
    // if(head == NULL)
    // {
    //     newOnline->next == NULL;
    //     head = newOnline;
    //     printf("Init online user success, name is %s\n", newOnline->username);
    // }
    // else
    // {
    //     newOnline->next = head->next;
    //     head->next = newOnline;
    //     printf("add online user success, name is %s\n", newOnline->username);
    // }
}

int findOnlineUser(char username[32])
{
    struct onlineUser *temp;
    temp = (struct onlineUser *)malloc(sizeof(struct onlineUser));
    temp = head->next;
    while(temp != NULL)
    {
        if(strcmp(username, temp->username) == 0)
        {
            printf("Find the user %s, clientFD is %d\n", username, temp->clientFD);
            return temp->clientFD;
        }
        temp = temp->next;
    }
    return -1;
}

int deleteOnlineUser(int closedClientFD)
{
    bool isDeleted = false;
    struct onlineUser* prevTemp;
    struct onlineUser* temp;
    prevTemp = (struct onlineUser *)malloc(sizeof(struct onlineUser));
    temp = (struct onlineUser *)malloc(sizeof(struct onlineUser));
    prevTemp = head;
    temp = head->next;
    while(temp != NULL)
    {
        if(temp->clientFD == closedClientFD)
        {
            prevTemp->next = temp->next;
            printf("Clint %d is offline now!\n", closedClientFD);
            isDeleted = true;
            break;
        }
        prevTemp = prevTemp->next;
        temp = temp->next;
    }
    if(isDeleted)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}


sqlite3* OpenDatabase(char* filename)
{
    sqlite3 *db;
    int openResult;

    openResult = sqlite3_open("UserDB.db", &db);
    if(openResult != SQLITE_OK)
    {
        printf("User database open error! %s", sqlite3_errmsg(db));
        exit(1);
    }
    printf("Database open success!\n");
    return db;
}

int checkUsernameAvailiable(sqlite3* db, char username[32])
{
    char selectUserSql[1024];
    char** selectedResult;
    char* selectError;
    int selectedRow = 0;
    int selectedColumn = 0;
    
    sprintf(selectUserSql, "select * from user where username = '%s'", username);
    
    sqlite3_get_table(db, selectUserSql, &selectedResult, &selectedRow, &selectedColumn, &selectError);

    printf("Selected %d rows!", selectedRow);

    if(selectedRow != 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int Regist(sqlite3* db, char username[32], char password[256])
{
    char registUserSql[1024];
    char** selectedResult;
    int selectedRow = 0;
    int selectedColumn = 0;
    char* selectError;

    memset(registUserSql, 0, sizeof(registUserSql));

    sqlite3_get_table(db, "select * from user", &selectedResult, &selectedRow, &selectedColumn, &selectError);

    int ID = atoi(selectedResult[(selectedColumn * selectedRow)]) + 1;


    int registResult;
    char* insertError;
    sprintf(registUserSql, "insert into user(ID, username, password, privilege) values(%d, '%s', '%s', 1)", ID, username, password);
    registResult = sqlite3_exec(db, registUserSql, NULL, NULL, &insertError);

    if(registResult != SQLITE_OK)
    {
        printf("Regist Error! %s", sqlite3_errmsg(db));
        return -1;
        exit(1);
    }

    return 0;
}

int Login(sqlite3* db, char username[32], char password[256])
{
    char loginSql[1024];
    char** selectedResult;
    int selectedRow = 0;
    int selectedColumn = 0;
    char* loginError;

    memset(loginSql, 0, sizeof(loginSql));

    sprintf(loginSql, "select * from user where userName = '%s' AND password = '%s'", username, password);

    int selectResult;
    selectResult = sqlite3_get_table(db, loginSql, &selectedResult, &selectedRow, &selectedColumn, &loginError);

    printf("Selected %d rows\n", selectedRow);
    if(strcmp(selectedResult[selectedColumn + 2], password) == 0)
    {
        printf("%s Login successfully!\n", username);
    }
    else
    {
        printf("Wrong password\n");
        return -1;
    }
    if(atoi(selectedResult[selectedRow * selectedColumn + 3]) == 0)
    {
        return 0;
    }
    if(atoi(selectedResult[selectedRow * selectedColumn + 3]) == 1)
    {
        return 1;
    }
    return -1;
}

void * receiveMessage(void * arg)
{
    printf("Server start receiving message!\n");

    int clientFD = ((struct receiveFunctionArgs *)arg)->clientFD;
    sqlite3* db = ((struct receiveFunctionArgs *)arg)->db;

    int receivedCount;
    
    struct receiveMessageFromClient *receivedMessage;
    receivedMessage = (struct receiveMessageFromClient *)malloc(sizeof(struct receiveMessageFromClient));
    

    while (1)
    {
        bzero(receivedMessage, sizeof(struct receiveMessageFromClient));

        if((receivedCount = recv(clientFD, receivedMessage, sizeof(struct receiveMessageFromClient), 0)) < 0)
        {
            perror("Receive error!");
            exit(1);
        }
        if(receivedCount == 0)
        {
            printf("Client %d has been shutdown\n", clientFD);
            deleteOnlineUser(clientFD);
            shutdown(clientFD, SHUT_RDWR);
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
            {
                printf("Received regist request!\n");
                printf("Regist username is %s\n", receivedMessage->username);
                int availiable = checkUsernameAvailiable(db, receivedMessage->username);
                if(availiable == -1)
                {
                    printf("Username has been taken by someone!\n");
                    struct sendMessageToClient *registError;
                    registError = (struct sendMessageToClient *)malloc(sizeof(struct sendMessageToClient));
                    registError->command = 0;
                    registError->result = -1;
                    strcpy(registError->message, "Username has been taken by someone!\n");

                    send(clientFD, registError, sizeof(struct sendMessageToClient), 0);
                }
                else
                {
                    int registResult;
                    registResult = Regist(db, receivedMessage->username, receivedMessage->password);
                    if(registResult == 0)
                    {
                        struct sendMessageToClient *registSuccess;
                        registSuccess = (struct sendMessageToClient *)malloc(sizeof(struct sendMessageToClient));

                        registSuccess->result = 0;
                        registSuccess->command = 0;

                        send(clientFD, registSuccess, sizeof(struct sendMessageToClient), 0);
                    }

                }
                break;
            }
            
        case 1:
            {
                printf("Received login request!\n");
                printf("Attempt to login username is %s\n", receivedMessage->username);

                int loginResult;
                loginResult = Login(db, receivedMessage->username, receivedMessage->password);
                if(loginResult != -1)
                {
                    struct onlineUser *newUser;
                    newUser = (struct onlineUser *)malloc(sizeof(struct onlineUser));
                    newUser->clientFD = clientFD;
                    strcpy(newUser->username,receivedMessage->username);
                    printf("new online user added, name is %s\n", newUser->username);
                    addOnlineUser(newUser);
                    struct sendMessageToClient *loginSuccessMessage;
                    loginSuccessMessage = (struct sendMessageToClient *)malloc(sizeof(struct sendMessageToClient));
                    loginSuccessMessage->command = 1;
                    loginSuccessMessage->result = 0;
                    send(clientFD, loginSuccessMessage, sizeof(struct sendMessageToClient), 0);
                    break;
                }
                else
                {
                    struct sendMessageToClient *loginFailedMessage;
                    loginFailedMessage = (struct sendMessageToClient *)malloc(sizeof(struct sendMessageToClient));
                    loginFailedMessage->command = 1;
                    loginFailedMessage->result = -1;
                    send(clientFD, loginFailedMessage, sizeof(struct sendMessageToClient), 0);
                    break;
                }
                break;
            }
            
        case 2:
            {
                printf("Received send message to person request!\n");
                printf("this message is send to %s\n", receivedMessage->sendToName);
                printf("this message is from %s\n", receivedMessage->username);
                struct sendMessageToClient *messageToPerson;
                messageToPerson = (struct sendMessageToClient *)malloc(sizeof(struct sendMessageToClient));
                messageToPerson->command = 2;
                messageToPerson->result = 0;
                strcpy(messageToPerson->receiveFromName, receivedMessage->username);
                strcpy(messageToPerson->message, receivedMessage->message);
                printf("Generate message to person success!\n");
                {
                    int sendToFD;
                    sendToFD = findOnlineUser(receivedMessage->sendToName);
                    if(sendToFD != -1)
                    {
                        send(sendToFD, messageToPerson, sizeof(struct sendMessageToClient), 0);
                        printf("Send message to person success!\n");
                        break;
                    }
                    else
                    {
                        messageToPerson->result = -1;
                        printf("Send message to person failed!\n");
                        send(clientFD, messageToPerson, sizeof(struct sendMessageToClient), 0);
                    }
                }
                break;
            }
            
            
        case 3:
            {
                printf("Received send broadcast message request!\n");
                struct sendMessageToClient *broadcastMessage;
                broadcastMessage = (struct sendMessageToClient *)malloc(sizeof(struct sendMessageToClient));
                broadcastMessage->command = 3;
                strcpy(broadcastMessage->message, receivedMessage->message);
                strcpy(broadcastMessage->receiveFromName, receivedMessage->username);
                broadcastMessage->result = 0;
                struct onlineUser *currentSendTo;
                currentSendTo = (struct onlineUser *)malloc(sizeof(struct onlineUser));
                currentSendTo = head->next;

                while(currentSendTo != NULL)
                {
                    if(strcmp(currentSendTo->username, receivedMessage->username) != 0)
                    {
                        send(currentSendTo->clientFD, broadcastMessage, sizeof(struct sendMessageToClient), 0);
                    }
                    currentSendTo = currentSendTo->next;
                }
                
                break;
            }
        
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

    sqlite3* db = OpenDatabase("./UserDB.db");
    
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

        struct receiveFunctionArgs *arg;
        arg = (struct receiveFunctionArgs *)malloc(sizeof(struct receiveFunctionArgs));
        bzero(arg, sizeof(struct receiveFunctionArgs));

        arg->clientFD = clientFD;
        arg->db = db;

        pthread_create(&id, NULL, receiveMessage, (void *)arg);

        usleep(3);
        
    }

    
    return 0;
}