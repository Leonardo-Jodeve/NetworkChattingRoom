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
    char data[] = "Hello, named pipe!";
    int writeCount;

    if(access(".fifo", F_OK) == -1)
    {
        if(mkfifo("./fifo", 0644) < 0)
        {
            printf("Create pipe error!\n");
            exit(1);
        }
    }

    fd = open("./fifo",O_WRONLY);
    if(fd == -1)
    {
        perror("Open read pipe error!\n");
        exit(1);
    }

    if((writeCount = write(fd, data, sizeof(data))) > 0)
    {
        printf("Write to pipe: %s\n", data);
    }

    close(fd);
    return 0;
}