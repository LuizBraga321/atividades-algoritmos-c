#include <stdio.h>

int main(void)
{
    printf("1 + 2 = %d\n", 1 + 2);
    printf("10 + 5/2 = %d\n", 10 + 5/2);
    printf("10 + 5.0/2 = %.1f\n", 10 + 5.0/2);
    printf("1 + 2*2 = %d\n", 1 + 2*2);
    printf("1 + 2.0/2*2 = %.1f\n", 1 + 2.0/2*2);
    printf("2*3 + 1 - 5*2 = %d\n", 2*3 + 1 - 5*2);
    return 0;
}
