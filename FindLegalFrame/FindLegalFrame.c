//
// Created by Leonardo Jodeve on 2021/3/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void FindLegalFrame(char* source, char* head, char* tail, char legalFrame[])
{
    char* headPointer;
    char* tailPointer;

    headPointer = (char *)malloc(sizeof(char) * 100);
    tailPointer = (char *)malloc(sizeof(char) * 100);
    if(headPointer == NULL || tailPointer == NULL)
    {
        printf("Malloc error!\n");
        exit(0);
    }

    headPointer = strstr(source,head);
    tailPointer = strstr(source,tail);

    if(headPointer == NULL || tailPointer == NULL || strlen(tailPointer) >= strlen(headPointer))
    {
        printf("Don't have any legal frame!\n");
    }
    else
    {
        strncat(legalFrame,headPointer,strlen(headPointer)-strlen(tailPointer));
        strncat(legalFrame,tail,strlen(tail));
    }
}

int main(int args, char* argv[])
{
    char* source = "000head1234tail000";
    char* head = "head";
    char* tail = "tail";
    char legalFrame[sizeof source];

    FindLegalFrame(source,head,tail,legalFrame);
    printf("%s",legalFrame);
}