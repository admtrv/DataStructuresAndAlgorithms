#include <stdlib.h>
#include <stdio.h>

unsigned long long euclAlg(unsigned long long a, unsigned long long b) 
{
    if (a == 0)
        return b;
    else
        return euclAlg(b % a, a);
}

int main() 
{
    int n = 0;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) 
    {
        unsigned long long a = 0;
        unsigned long long b = 0;

        scanf("%llu %llu", &a, &b);
        
        if ( i + 1 < n) 
            printf("%llu\n", euclAlg(a, b));
        else 
            printf("%llu", euclAlg(a, b));
    }
    
    return 0;
}