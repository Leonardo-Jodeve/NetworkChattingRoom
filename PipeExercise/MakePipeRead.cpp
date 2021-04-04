#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    int createPipeResult;

    char buffer[512];

    // if((createPipeResult = mkfifo("./file", 0644) < 0))
    // {
    //     if(createPipeResult == -1 && errno == EEXIST)
    //     {
            
    //     }
    //     else
    //     {
    //         perror("Make pipe error!");
    //         exit(1);
    //     }
    // }

    int fd = open("./file", O_RDONLY);
    if(fd == -1)
    {
        perror("Read pipe error!");
        exit(1);
    }

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        read(fd, buffer, sizeof(buffer));
        printf("Receved:%s", buffer);
        sleep(1);
    }
    

    return 0;
}