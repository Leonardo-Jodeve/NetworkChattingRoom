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
    int fd = -1;
    char buffer[1024];
    int readCount;

    fd = open("./fifo",O_RDONLY);

    if(fd == -1)
    {
        perror("Open read pipe error!\n");
        exit(1);
    }

    if((readCount = read(fd, buffer, sizeof(buffer))) > 0)
    {
        printf("Read from pipe: %s\n", buffer);
    }

    close(fd);
    return 0;
}