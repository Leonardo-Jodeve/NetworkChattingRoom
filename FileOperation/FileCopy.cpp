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

#define BUFFER_LENGTH 1024

void FileCopy(char* sourceFilePath, char* destinationFilePath)
{
    int sourceFD, destinationFD;
    int bytesRead, bytesWrite;
    char buffer[BUFFER_LENGTH];
    char* currentPosition;

    sourceFD = open(sourceFilePath, O_RDONLY);
    destinationFD = open(destinationFilePath, O_RDWR | O_CREAT, 0644);

    if(sourceFD == -1)
    {
        perror("Read error!");
        exit(1);
    }
    if(destinationFD == -1)
    {
        perror("Write error!");
        exit(1);
    }

    while(bytesRead = read(sourceFD, buffer, BUFFER_LENGTH))
    {
        if(bytesRead == -1)
        {
            break;
        }
        if(bytesRead > 0)
        {
            currentPosition = buffer;
            while(bytesWrite = write(destinationFD, currentPosition, bytesRead))
            {
                if(bytesWrite == -1)
                {
                    break;
                }
                else if(bytesWrite == bytesRead)
                {
                    break;
                }
                else if(bytesWrite > 0)
                {
                    currentPosition = currentPosition + bytesWrite;
                    bytesRead = bytesRead - bytesWrite;
                }
            }
        }
        if(bytesWrite == -1)
        {
            break;
        }
        memset(buffer, 0, BUFFER_LENGTH);
    }
    close(sourceFD);
    close(destinationFD);
}

int main(int argv, char* args[])
{
    if(argv != 3)
    {
        printf("Arguments error!");
        exit(0);
    }
    FileCopy(args[1], args[2]);
}