#include <stdio.h>

int main()
{
    int i;
    int sum = 0;

    for(i = 0; i<=100; i++)
    {
        sum = sum + i;
    }

    printf("sum = %d\n",sum);
    return 0;
}