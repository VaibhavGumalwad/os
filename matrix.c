3)matrix multiplication
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 10

// Structure to pass arguments to threads
typedef struct {
    int row;
    int col;
    int size;
    int (*matrix1)[MAX_SIZE];
    int (*matrix2)[MAX_SIZE];
    int (*result)[MAX_SIZE];
} ThreadArgs;

// Function to perform row-column multiplication
void* multiply(void* arguments) {
    ThreadArgs* args = (ThreadArgs*)arguments;

    int row = args->row;
    int col = args->col;
    int size = args->size;

    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += args->matrix1[row][i] * args->matrix2[i][col];
    }

    int* result = malloc(sizeof(int));
    *result = sum;
    pthread_exit(result);
}

int main() {
    int size;

    // Input the size of the matrices
    printf("Enter the size of the matrices: ");
    scanf("%d", &size);

    if (size > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix size exceeds the maximum allowed size.\n");
        exit(EXIT_FAILURE);
    }

    int matrix1[MAX_SIZE][MAX_SIZE];
    int matrix2[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];

    // Input the first matrix
    printf("Enter elements of the first matrix:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &matrix1[i][j]);
        }
    }

    // Input the second matrix
    printf("Enter elements of the second matrix:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &matrix2[i][j]);
        }
    }

    // Create an array of pthreads
    pthread_t threads[MAX_SIZE][MAX_SIZE];

    // Create thread arguments
    ThreadArgs args[MAX_SIZE][MAX_SIZE];

    // Create threads for matrix multiplication
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            args[i][j].row = i;
            args[i][j].col = j;
            args[i][j].size = size;
            args[i][j].matrix1 = matrix1;
            args[i][j].matrix2 = matrix2;
            args[i][j].result = result;

            pthread_create(&threads[i][j], NULL, multiply, &args[i][j]);
        }
    }

    // Collect the results from threads and perform final sum
    int totalSum = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int* resultPtr;
            pthread_join(threads[i][j], (void**)&resultPtr);
            result[i][j] = *resultPtr;
            totalSum += result[i][j];
            free(resultPtr);
        }
    }

    // Display the result matrix
    printf("Result matrix:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    printf("Total sum of the result matrix: %d\n", totalSum);

    return 0;
}


===========================ooooooooooooooooooooorrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr===============


Matrix1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define SIZE 10
int A[SIZE][SIZE], B[SIZE][SIZE];
long int C[SIZE][SIZE];
void *mul_thread(void *arg) {
int i, row, col, *rcArgs;
long int return_val; //long int, since int cannot be type casted to void
rcArgs = (int *) arg;
row = rcArgs[0];
col = rcArgs[1];
i = rcArgs[2];
return_val = A[row][i] * B[i][col];
//return ((void *) return_val);
pthread_exit((void *) return_val);
}
void accept_matrix(int M[SIZE][SIZE], int rows, int cols) {
int i, j;
printf("\n");
for(i=0;i<rows;i++) {
for(j=0;j<cols;j++) {
printf("Value at [%d][%d]: ",i+1,j+1);
scanf("%d",&M[i][j]);
}
}
}
void display_matrix(int M[SIZE][SIZE], int rows, int cols) {
int i, j;
printf("\n");
for(i=0;i<rows;i++){
for(j=0;j<cols;j++){
printf("%2d ",M[i][j]);
}
printf("\n");
}
}
int main() {
int rows_A, cols_A, rows_B, cols_B;
int rcArgs[3];
int i, j, k, *status;
pthread_t P[SIZE][SIZE][SIZE];
printf("\nEnter no. of rows in matrix A: ");
scanf("%d",&rows_A);
printf("Enter no. of columns in matrix A: ");
scanf("%d",&cols_A);
accept_matrix(A, rows_A, cols_A);
printf("\nEnter no. of rows in matrix B: ");
scanf("%d",&rows_B);
printf("Enter no. of columns in matrix B: ");
scanf("%d",&cols_B);
accept_matrix(B, rows_B, cols_B);
if(cols_A == rows_B) {
for(i=0;i<rows_A;i++) {
for(j=0;j<cols_B;j++) {
for(k=0;k<cols_A;k++){
rcArgs[0] = i;
rcArgs[1] = j;
rcArgs[2] = k;
//Creating a new thread for every multiplication operation
if(pthread_create(&P[i][j][k], NULL, mul_thread, rcArgs) !=
0)
printf("\nCannot create thread.\n");
else
//Inserting delay
sleep(1);
}
}
}
} else {
printf("\n Matrix multiplication not possible.");
exit(1);
}
printf("\nMatrix A:");
display_matrix(A, rows_A, cols_A);
printf("\nMatrix B:");
display_matrix(B, rows_B, cols_B);
for(i=0;i<rows_A;i++) {
for(j=0;j<cols_B;j++) {
for(k=0;k<cols_A;k++){
//joining all the threads and retrieving values in status
if(pthread_join(P[i][j][k],(void **) &status) != 0)
perror("\nThread join failed.\n");
C[i][j] += (long int)status;
}
}
}
printf("\nResultant Matrix:\n");
for(i=0;i<rows_A;i++){
for(j=0;j<cols_B;j++){
printf("%2ld ",C[i][j]);
}
printf("\n");
}
exit(EXIT_SUCCESS);
}
