#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sieveEratos(int a, int b) 
{
    if (a < 2) 
    {
        a = 2;
    }
    
    char *isPrime = (char *)malloc(b+1);
    memset(isPrime, 1, b+1);

    for (int i = 2; i * i <= b; i++) 
    {
        if (isPrime[i]) 
        {
            for (int j = i * i; j <= b; j += i)
                isPrime[j] = 0;
        }
    }

    int firstFlag = 0;
    for (int i = a; i <= b; i++) 
    {
        if (isPrime[i]) 
        {
            if (firstFlag)
            {
                printf(", ");
            }
            printf("%d", i);
            firstFlag = 1;
        }
    }

    free(isPrime);
}

int main() 
{

    int a, b;
    scanf("%d %d", &a, &b);

    sieveEratos(a, b);

    return 0;
}
