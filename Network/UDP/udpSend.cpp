#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 32767

int main(int argc, char* argv[])
{
    int socketFD;

    struct sockaddr_in destAddress, myAddress;

    char buffer[1024];

    int sendCount;
    
    socketFD = socket(AF_INET, SOCK_DGRAM, 0);

    if(socketFD < 0)
    {
        perror("Socket create error!\n");
        exit(1);
    }

    int opt = 1;

    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // bzero(&myAddress, sizeof(struct sockaddr_in));
    // myAddress.sin_family = AF_INET;
    // myAddress.sin_port = htons(PORT);
    // myAddress.sin_addr.s_addr = inet_addr("192.168.61.130");

    // int bindResult;
    // bindResult = bind(socketFD, (struct sockaddr *)(&myAddress), sizeof(struct sockaddr_in));
    // if(bindResult < 0)
    // {
    //     perror("Bind error!\n");
    //     exit(1);
    // }


    bzero(&destAddress, sizeof(struct sockaddr_in));
    destAddress.sin_family = AF_INET;
    destAddress.sin_port = htons(32768);
    destAddress.sin_addr.s_addr = inet_addr("192.168.61.130");

    
    while(1)
    {   
        bzero(&destAddress, sizeof(struct sockaddr_in));
        destAddress.sin_family = AF_INET;
        destAddress.sin_port = htons(32768);
        destAddress.sin_addr.s_addr = inet_addr("192.168.61.130");
        
        memset(buffer, 0, sizeof(buffer));
        printf("Please input send message:\n");
        scanf("%s", buffer);
        sendCount = sendto(socketFD, buffer, strlen(buffer), 0, (struct sockaddr *)&destAddress, sizeof(struct sockaddr_in));
        printf("Sendcount = %d\n", sendCount);
        
        usleep(3);
    }

    return 0;
}