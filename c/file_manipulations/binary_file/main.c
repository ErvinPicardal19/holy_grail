#include <stdio.h>
#include <stdlib.h>
void write_binary_file();
void read_binary_file();
int main(int argc, char* argv[])
{

    int input;
    
    printf("1 - Write binary file\n2 - Read binary file\nEnter: ");
    scanf("%d", &input);

    switch(input)
    {
        case 1:
            write_binary_file();
        break;
        case 2:
            read_binary_file();
        break;
    }

    return 0;
}

void write_binary_file()
{
    int data[] = {1000000000,
                  1000000001,
                  1000000002,
                  1000000003,
                  1000000004,
                  1000000005,
                  1000000006,
                  1000000007,
                  1000000008,
                  1000000009};

    FILE *file;

    file = fopen("file.bin", "wb");

    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    fwrite(data, sizeof(data), 1, file);

    fclose(file);
}

void read_binary_file()
{ 
    int data[10];
    FILE *file;

    file = fopen("file.bin", "rb");

    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    fread(data, sizeof(data), 1, file);

    fclose(file);

    for (int i = 0; i < 10; i ++)
    {
        printf("data[%d] = %d\n", i, data[i]);
    }
}

