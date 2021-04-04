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


    if((createPipeResult = mkfifo("./file", 0644) < 0))
    {
        if(createPipeResult == -1 && errno == EEXIST)
        {
            
        }
        else
        {
            perror("Make pipe error!");
            exit(1);
        }
    }

    int fd = open("./file", O_WRONLY);
    if(fd == -1)
    {
        perror("Open pipe error!");
        exit(1);
    }

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        scanf("%s", buffer);
        write(fd, buffer, strlen(buffer));
    }
    
    return 0;
}