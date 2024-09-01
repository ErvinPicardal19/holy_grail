#include <stdio.h>

int add(int x, int y)
{
    return x + y;
}

int subtract(int x, int y)
{
    return x - y;
}

int divide(int x, int y)
{
    return x / y;
}

int multiply(int x, int y)
{
    return x * y;
}

int (*operation(unsigned char choice))(int, int)
{
    switch(choice)
    {
        case '1':
            return add;
        break;
        case '2':
            return subtract;
        break;
        case '3':
            return divide;
        break;
        case '4':
            return multiply;
        break;
    }

    return NULL;
}
                            

void foo(double (*func)(double, double))
{
    double x = func(7.0, 5.0);
    printf("SUM: %lf\n", x);
}

int main(void)
{
    unsigned char choice;
    puts("Choose which operation to do:");
    puts("1. Add");
    puts("2. Subtract");
    puts("3. Divide");
    puts("4. Multiply");
    printf("Enter: ");
    scanf("%c", &choice);

    int (*func)(int, int) = operation(choice);

    if(func == NULL) {
        fprintf(stderr, "Invalid Operation\n");
        return 1;
    }
    
    char *operation_name = choice == '1' ? "Add" :
                                choice == '2' ? "Subtract" :
                                    choice == '3' ? "Divide" :
                                        choice == '4' ? "Multiply" : '\0';

    printf("Operation: %s\n", operation_name);
    printf("Result: %d\n", func(10, 5));

    return 0;
}
