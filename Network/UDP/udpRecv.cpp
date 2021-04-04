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

#define PORT 32768

int main(int argc, char* argv[])
{
    int socketFD;

    struct sockaddr_in srcAddress;
    int sockaddrLength = sizeof(struct sockaddr_in);

    char buffer[1024];
    int receivedCount;
    socketFD = socket(AF_INET, SOCK_DGRAM, 0);

    if(socketFD < 0)
    {
        perror("Socket create error!\n");
        exit(1);
    }
    
    int opt = 1;

    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&srcAddress, sizeof(struct sockaddr_in));
    srcAddress.sin_family = AF_INET;
    srcAddress.sin_port = htons(PORT);
    //srcAddress.sin_addr.s_addr = inet_addr("192.168.61.130");
    
    int bindResult;
    bindResult = bind(socketFD, (struct sockaddr *)(&srcAddress), sizeof(struct sockaddr_in));
    if(bindResult < 0)
    {
        perror("Bind error!\n");
        exit(1);
    }
    
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        
        receivedCount = recvfrom(socketFD, buffer, sizeof(buffer), 0, NULL, NULL);
        printf("Received count:%d", receivedCount);
        printf("Received text:%s", buffer);

        sleep(1);
    }

    return 0;
}