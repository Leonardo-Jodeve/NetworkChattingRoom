#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int readFD, writeFD;

    int readCount, writeCount;

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    readFD = open("./src.txt", O_RDONLY);
    if(readFD < 0)
    {
        perror("Open source file error!\n");
        exit(1);
    }
    lseek(readFD, -10240, SEEK_END);

    writeFD = open("./dest.txt", O_WRONLY | O_CREAT, 0644);
    if(writeFD < 0)
    {
        perror("Open dest file error!\n");
        exit(1);
    }

    while((readCount = read(readFD, buffer, sizeof(buffer))) > 0)
    {
        write(writeFD, buffer, readCount);
        printf("Write %d bytes!\n", readCount);
    }

    close(readFD);
    close(writeFD);

    return 0;

}