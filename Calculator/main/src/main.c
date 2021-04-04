#include <stdio.h>
#include <math.h>

extern int add(int,int);
extern int sub(int,int);
extern double multiply(double,double);
extern double divide(double,double);

int main()
{
    int a = 5;
    int b = 4;

    double x = 3.1;
    double y = 6.5;

    printf("add(%d + %d) = %d\n",a,b,add(a,b));
    printf("sub(%d + %d) = %d\n",a,b,sub(a,b));
    printf("mul(%lf + %lf) = %lf\n",x,y,multiply(x,y));
    printf("div(%lf + %lf) = %lf\n",x,y,divide(x,y));

    return 0;
}