#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void MaxIntegerInString(char* string, int length, char* result)
{
    int head = 0;
    int tail = 0;
    int maxHead = 0;
    int maxTail = 0;

    int subLength = 0;
    int maxLength = 0;

    for(int i = 0; i < length; i++)
    {
        if(*(string+i) >= '0' && *(string+i) <= '9')
        {
            head = i;
            tail = i;
            while (*(string+tail) >= '0' && *(string+tail) <= '9')
            {
                tail++;
            }
            subLength = tail - head;

            if(subLength > maxLength)
            {
                maxLength = subLength;
                maxHead = head;
                maxTail = tail;
            }
            i = tail - 1;
        }
    }
    strncpy(result,(string + maxHead), maxTail - maxHead);
}

int main()
{
    char* string;
    string = (char *)malloc(sizeof(char) * 100);
    char* result;
    result = (char *)malloc(sizeof(char) * 100);
    printf("Please input string:");
    gets(string);
    MaxIntegerInString(string,strlen(string),result);
    printf("Max integer in this string is: %s\n",result);
    printf("Max integer length in this string is: %d\n",strlen(result));
    free(string);
    free(result);
    return 0;
}