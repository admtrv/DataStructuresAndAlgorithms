#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long int whole;
    long long int decimal;
    int sign;
} RationalNumber;

char* findFirstChar(char* string, int target) 
{
    while (*string != '\0')
    {
        if (*string == target) 
            return string;
        
        string+=1;
    }

    return NULL;
}

int stringLength(char* string) 
{
    int c = 0;

    while (*string != '\0') 
    {
        c+=1;
        string+=1;
    }

    return c;
}

long long int fromStringToInteger(char* string) 
{
    long long int result = 0;

    if (*string == '-') 
        string+=1;

    while (*string >= '0' && *string <= '9') 
    {
        result = result * 10 + (*string - '0');
        string+=1;
    }

    return result;
}

void removeNewline(char* string) 
{
    while (*string) 
    {
        if (*string == '\n') 
        {
            *string = '\0';
            break;
        }
        string+=1;
    }
}

void readRationalNumber(char* input, RationalNumber* num) 
{
    char* dotPos = findFirstChar(input, ',');
    
    num->sign = (*input == '-')? -1 : 1;

    if (dotPos != NULL) 
    {
        *dotPos = '\0'; 

        num->whole = fromStringToInteger(input);
        
        char* decimalPart = dotPos + 1;
        int leng = stringLength(decimalPart);

        for (int i = leng - 1; i >= 0 && decimalPart[i] == '0'; i--) 
        {
            decimalPart[i] = '\0';
            leng -= 1;
        }

        for (int i = leng; i < 10; i++)
        {
            decimalPart[i] = '0';
        }
        decimalPart[10] = '\0';
        
        num->decimal = fromStringToInteger(decimalPart);

    } else 
    {
        num->whole = fromStringToInteger(input);
        num->decimal = 0;
    }
}

int compareRationalNumbers(RationalNumber a, RationalNumber b) 
{
    if (a.sign > b.sign) return 1;
    if (a.sign < b.sign) return -1;

    if (a.whole > b.whole) return a.sign; 
    if (a.whole < b.whole) return -a.sign;

    if (a.decimal > b.decimal) return a.sign;
    if (a.decimal < b.decimal) return -a.sign;

    return 0;
}

int binarySearch(RationalNumber* array, int size, RationalNumber target)
{
    int lowerPoint = 0;
    int upperPoint = size - 1;
    int midPoint = 0;
    
    while (lowerPoint <= upperPoint)
    {
        midPoint = (upperPoint + lowerPoint) / 2;

        int c = compareRationalNumbers(array[midPoint], target);

        if (c == 0) 
            return midPoint + 1;
        else if (c < 0) 
            lowerPoint = midPoint + 1;
        else 
            upperPoint = midPoint - 1;
    }
    return 0;
}

int main() 
{
    int n;
    scanf("%d", &n);
    getchar();

    RationalNumber* array = (RationalNumber*)malloc(n * sizeof(RationalNumber));

    char inputString[25];
    for (int i = 0; i < n; i++)
    {
        fgets(inputString, 25, stdin);
        removeNewline(inputString);

        readRationalNumber(inputString, &array[i]);
    }

    int m;
    scanf("%d", &m);
    getchar();

    RationalNumber target;

    for (int i = 0; i < m; i++) 
    {
        fgets(inputString, 25, stdin);
        removeNewline(inputString);

        readRationalNumber(inputString, &target);

        if (i < m - 1) 
            printf("%d\n", binarySearch(array, n, target));
        else 
            printf("%d", binarySearch(array, n, target));
    }

    free(array);

    return 0;
}
