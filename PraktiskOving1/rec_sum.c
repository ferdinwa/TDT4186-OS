#include <stdio.h>

char b = 'a';
double d;

int sum_n(int n) {
    if (n == 0)
        return n;
    else
        return  n + sum_n(n - 1);
}

int main(void) {
    int n = 65143;
    char *p = "1234";
    int *s;
    printf("The sum of numbers from %d to %d is %d\n", 1, n, sum_n(n));
    printf("The adress of n is %p\n", &n);
    //printf("Address of b is %p\n", &b);
    printf("Address of p is %p\n", &p);
    //printf("Address of d is %p\n", &d);
    printf("Address of s is %p\n", &s);
    return 0;
}