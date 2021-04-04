//
// Created by Leonardo Jodeve on 2021/3/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LENGTH 500


int * CountCharacters(char string[], int length)
{
    static int characterCount[128] = {0};

    for(int i = 0; i < length; i++)
    {
        int index = string[i] - '\0';
        characterCount[index]++;
    }

    return characterCount;
}

int * FindMinNumberInArray(int (*array), int length)
{
    static int minIndex[MAX_LENGTH] = {0};
    int minValue = INT_MAX;
    int index = 0;

    for(int i = 0; i < length; i++)
    {
        if(*(array+i) < minValue && *(array+i) != 0)
        {
            minValue = *(array+i);
        }
    }

    for(int i = 0; i < length; i++)
    {
        if(*(array+i) == minValue)
        {
            minIndex[index] = i;
            index++;
        }
    }
    minIndex[index] = -1;
    return minIndex;
}

void DeleteCharacterFromString(char string[], int length, int indexToDelete[])
{
    int i = 0;
    while(indexToDelete[i] != -1)
    {
        for(int j = 0; j < length; j++)
        {
            if(string[j] - indexToDelete[i] == 0)
            {
                string[j] = '\7';
            }
        }
        i++;
    }
}

void CleanString(char string[], int length)
{
    char temp;
    int j = 0;
    for(int i = 0; i < length; i++)
    {
        j = i;
        if(string[i] != '\7')
        {
            temp = string[i];
            string[i] = '\7';
            while(j > 0 && string[j-1] == '\7')
            {
                j--;
            }
            string[j] = temp;
        }
    }
}

int main()
{
    char string[20] = "Hello World!";
    DeleteCharacterFromString(string,strlen(string),FindMinNumberInArray(CountCharacters(string,strlen(string)),128));
    CleanString(string,strlen(string));
    printf("%s\n",string);

    return 0;
}