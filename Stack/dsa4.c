#include <stdio.h>
#include <stdlib.h>

int isMatching(char a, char b) 
{
    if (a == '(' && b == ')')
        return 1;
    else if (a == '[' && b == ']')
        return 1;
    else if (a == '{' && b == '}')
        return 1;
    else if (a == '<' && b == '>')
        return 1;
    return 0;
}

int analyzeBrackets()
{
    char* stack = (char*)malloc(100000 * sizeof(char));
    int top = -1;
    int errorCount = 0;

    char ch; // Current char
    while (scanf("%c", &ch) > 0 && ch != '\n') // Read char to the end of the string
    {
        if (ch == '(' || ch == '[' || ch == '{' || ch == '<') 
        {
            top+=1;
            stack[top] = ch;
        } 
        else 
        {
            if (top == -1) // If the stack is empty
            { 
                errorCount+=1;
            } 
            else if (!isMatching(stack[top], ch)) // If the brackets are different
            {
                errorCount+=1; 
                top-=1; 
            } 
            else // Else we found a complete pair of brackets
            {
                top-=1;
            }
        }
    }

    errorCount += (top + 1); // The rest of the brackets are errors

    free(stack);

    return errorCount;
}

int main() 
{
    int n;
    scanf("%d\n", &n); 

    for (int i = 0; i < n; i++) 
    {
        if (i < n - 1)
            printf("%d\n", analyzeBrackets());
        else
            printf("%d", analyzeBrackets());
    }

    return 0;
}