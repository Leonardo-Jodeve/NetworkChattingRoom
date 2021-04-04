//
// Created by Leonardo Jodeve on 2021/3/31.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int fatherFD, childFD;

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("Create child thread failed!\n");
        exit(1);
    }
    else if(pid > 0)
    {
        char text[] = "Gettysburg Address\n"
                      "\n"
                      "Abraham Lincoln\n"
                      "\n"
                      "Four score and seven years ago our fathers brought forth on "
                      "this continent, a new nation, conceived in Liberty, and dedicated to "
                      "the proposition that all men are created equal.\n"
                      "Now we are engaged in a great civil war, testing whether that nation, "
                      "or any nation so conceived and so dedicated, can long endure. "
                      "We are met on a great battle-field of that war. "
                      "We have come to dedicate a portion of that field, "
                      "as a final resting place for those who here gave their lives that nation might live."
                      " It is altogether fitting and proper that we should do this.\n"
                      "But, in a larger sense, we can not dedicate—we can not consecrate—we can not hallow—this ground."
                      " The brave men, living and dead, who struggled here, have consecrated it, "
                      "far above our poor power to add or detract. The world will little note, "
                      "nor long remember what we say here, but it can never forget what they did here."
                      " It is for us the living, rather, to be dedicated here to the unfinished "
                      "work which they who fought here have thus far so nobly advanced. It is rather for us "
                      "to be here dedicated to the great task remaining before us—that from these honored dead "
                      "we take increased devotion to that cause for which they gave the last full measure of "
                      "devotion—that we here highly resolve that these dead shall not have died in vain—that "
                      "this nation, under God, shall have a new birth of freedom—and that government of the "
                      "people, by the people, for the people, shall not perish from the earth.";
        fatherFD = open("source.txt", O_RDWR | O_CREAT, 0644);
        if(fatherFD == -1)
        {
            perror("Father creat/open file error!\n");
            exit(1);
        }
        write(fatherFD, text, strlen(text));
        close(fatherFD);
        wait(NULL);
    }
    else if(pid == 0)
    {
        sleep(1);
        childFD = open("source.txt", O_RDONLY);
        char buffer[4096];
        if(childFD == -1)
        {
            perror("Child read file error!\n");
            exit(1);
        }
        printf("Here are what I read from father:\n");
        while(read(childFD, buffer, 4096))
        {
            printf("%s", buffer);
        }
        close(childFD);
    }
}