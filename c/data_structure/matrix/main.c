#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int** create_matrix(const int n, const int m);
void delete_matrix(int **matrix);

int main()
{

    int *(*matrix) = create_matrix(2, 4);
    if (matrix == NULL) {
        printf("Memory Allocation Failed.\n");
        return 1;
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d\t", *(*(matrix + i)+j));
        }
        puts("");
    }

    delete_matrix(matrix);
    matrix=NULL;
    return 0;
}

void delete_matrix(int **matrix)
{
    for (int i = 0; i < 2; i++)
    {
        free(*(matrix + i));
    }

    free(matrix);
}

int** create_matrix(const int n, const int m)
{
    int *(*rows) = (int**)malloc(n * sizeof(int*));
    if (rows == NULL) {
        return NULL;
    }

    for(int i = 0; i < 2; i++)
    {
        *(rows+i) = (int*)calloc(n, sizeof(int));
        if (*(rows+i) == NULL) {
            return NULL;
        }
    }

    return rows;
}
