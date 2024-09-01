#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool isValid(char* s) 
{
    size_t size = strlen(s); 
    char stack[size];
    char prev = 0;
    int top = 0;
    for(int i = 0; i < size; i++)
    {

        if(s[i] == '(' || s[i] == '[' || s[i] == '{') {
            stack[top] = s[i];

            prev = stack[top];
            printf("Current Top: %c\n", prev);
            top++;
        } else {
            if(s[i] == ')' && s[i] != prev+1)
            {
                return false;
            } else {
                if(s[i] != prev+2)
                    return false;
            }
        }
    }
}

int main(void) 
{
    char* test_cases[] = {"()","()[]{}","(]"};
    size_t size = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for(int i = 0; i < size; i++)
    {
        printf("isValid: %d\n", isValid(test_cases[i]));
    }

    return 0;
}


