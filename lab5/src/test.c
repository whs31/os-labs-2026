#include <stdio.h>
void swap(long *a, long *b);
unsigned long min(unsigned long a, unsigned long b);
unsigned long pow(unsigned long x, unsigned long p);
int main(void) {
    long a = 3, b = 5;
    swap(&a, &b);
    printf("swap(3, 5) -> a=%ld b=%ld\n", a, b);
    printf("min(7, 4)  = %lu\n", min(7, 4));
    printf("min(2, 9)  = %lu\n", min(2, 9));
    printf("pow(2, 10) = %lu\n", pow(2, 10));
    printf("pow(5, 0)  = %lu\n", pow(5, 0));
    printf("pow(0, 3)  = %lu\n", pow(0, 3));
    return 0;
}
