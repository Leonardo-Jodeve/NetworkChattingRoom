//
// Created by Leonardo Jodeve on 2021/3/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int ReadLine(int fd, char* buffer, int maxLength)
{
    int i;
    int readResult;
    char temp;

    for(i = 0; i < maxLength; i++)
    {
        readResult = read(fd, &temp, 1);

        if(readResult < 0)
        {
            perror("Read error!");
        }

        if(readResult == 0)
        {
            break;
        }

        if(temp == '\n')
        {
            break;
        }
        buffer[i] = temp;
    }

    buffer[i] = '\0';

    return i;
}


int ReadWrite(char* filePath)
{
    int fd = open(filePath, O_RDWR | O_CREAT, 0655);

    if(fd == -1)
    {
        perror("Open file error!");
        exit(1);
    }

    char buffer[1024];

    for(int i = 0; i < 3; i++)
    {
        printf("Please input string[%d]:",i);
        scanf("%s", buffer);

        if(write(fd, buffer, strlen(buffer)) < 0)
        {
            perror("Write error!");
            exit(1);
        }
        memset(buffer, 0, sizeof(buffer));
        write(fd, "\n", 1);
    }

    memset(buffer, 0, sizeof(buffer));
    lseek(fd, 0, SEEK_SET);
    while(ReadLine(fd, buffer, sizeof(buffer) - 1) != 0)
    {
        printf("%s\n",buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    close(fd);
    return 0;
}


int main(int argc, char* argv[])
{
    ReadWrite(argv[1]);
}