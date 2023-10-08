#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "symnmf.h"
#define SEEK_SET 0
#define LINE_SIZE 1000
#define EPSILON 1e-4
#define MAX_ITER 300
#define BETA 0.5

struct fileData
{
    double** X;
    int n;
    int d;
};

/* Function to calculate the similarity matrix A */
void sym(double** X, int n, int d, double** A) {
    int i;
    int j;
    double distance;
    double diff;
    int k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != j) {
                distance = 0.0;
                for (k = 0; k < d; k++) {
                    diff = X[i][k] - X[j][k];
                    distance += diff * diff;
                }
                A[i][j] = exp(-distance / 2.0);
            } else {
                A[i][j] = 0.0;
            }
        }
    }
}

/* Function to calculate the Diagonal Degree Matrix D*/
void ddg(double** A, int n, double** D) {
    int i;
    int j;
    double sum;
    for (i = 0; i < n; i++) {
        sum = 0.0;
        for (j = 0; j < n; j++) {
            if(i != j){
                D[i][j] = 0.0;
            }
            sum += A[i][j];
        }
        D[i][i] = sum;
    }
}

/* Function to calculate the normalized similarity matrix W */
void norm(double** A, double** D, int n, double** W) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            W[i][j] = A[i][j] / sqrt(D[i][i] * D[j][j]);
        }
    }
}

/*print matrix */
void printMatrix(double** matrix, int n, int d) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < d; j++) {
            if (j < d - 1){
                printf("%.4f,", matrix[i][j]);
            }
            else{
                printf("%.4f", matrix[i][j]);
            }
        }
        printf("\n");
    }
}
/* print and exit*/
void printAndExit(void) {
    printf("An Error Has Occurred");
    exit(1);
}
/* free matrix */
void freeMatrix(double** matrix, int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
/* malloc matrix */
double** mallocMatrix(int n, int d){
    double** matrix;
    int i;
    matrix = (double**)malloc(n * sizeof(double*));
    if (matrix == NULL)
    {
        printAndExit();
    }
    for (i = 0; i < n; i++)
    {
        matrix[i] = (double*)malloc(d * sizeof(double));
        if (matrix[i] == NULL)
        {
            printAndExit();
        }  
    }
    return matrix;
}
/* read file */
struct fileData read_file(char* file_name){
    struct fileData data;
    char line[LINE_SIZE];  /* Assuming a maximum line length of 1000 characters*/
    FILE *file;
    double **X;
    int i;
    int j;
    int n;
    int d;
    char *token;
    double value;
    int success;
    n = 0; 
    d = 0;
    success = 1;

    /* Read data from file */
    file = fopen(file_name, "r");
    if (file == NULL) {
        printAndExit();
    }
    while(fgets(line, sizeof(line), file) != NULL){
        if(n == 0){
            token = strtok(line, ",");
            while (token != NULL) {
                d++;
            token = strtok(NULL, ",");
            }
        }
        n++;
    }
    success = fseek(file, 0, SEEK_SET);
    if (success != 0)
    {
        printAndExit();
    }
    /* Allocate memory for the matrix dynamically */
    X = mallocMatrix(n, d);
    i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        j = 0;
        token = strtok(line, ",");

        while (token != NULL) {
        /* Convert the token to a double and store it in the row */
            value = atof(token);
            /* Store the value in the row */
            X[i][j] = value;
            token = strtok(NULL, ",");
            j++;
            if(i > n){
                break;
            }  
        }
        i++;
    }
    fclose(file);
    data.n = n;
    data.d = d;
    data.X = X;
    return data;
}
/* executed goal */
void executed_goal(char* file_name, char* goal){
    double** X;  /* Data matrix (n x d) */
    double** A;  /* Similarity matrix (n x n) */
    double** D;  /* Degree matrix (n x n) */
    double** W;  /* Normalized similarity matrix (n x n) */
    int n;
    int d;
    struct fileData data;
    
    data = read_file(file_name);
    n = data.n;
    d = data.d;
    X = data.X;
    /* Allocate memory for matrices*/

    A = mallocMatrix(n, n);
    
    /* Calculate matrices based on the specified goal */
    if (strcmp(goal, "sym") == 0) {
        /* Calculate similarity matrix A */
        sym(X, n, d, A);
        printMatrix(A, n, n);
    } else if (strcmp(goal, "ddg") == 0) {
        /* Calculate degree matrix D */
        D = mallocMatrix(n, n);
        sym(X, n, d, A);
        ddg(A, n, D);
        printMatrix(D, n, n);
        freeMatrix(D, n);
    } else if (strcmp(goal, "norm") == 0) {
        /* Calculate normalized similarity matrix W */
        /* Calculate degree matrix D */
        D = mallocMatrix(n, n);
        W = mallocMatrix(n, n);
        sym(X, n, d, A);
        ddg(A, n, D);
        norm(A, D, n, W);
        printMatrix(W, n, n);
        freeMatrix(D, n);
        freeMatrix(W, n);
    } else {
        freeMatrix(A, n);
        freeMatrix(X, n);
        printAndExit();
    }

    /* Free allocated memory */
    freeMatrix(X, n);
    freeMatrix(A, n);
}

void copyMatrix(double** sourceMatrix, double** destinationMatrix, int rows, int cols) {
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            destinationMatrix[i][j] = sourceMatrix[i][j];
        }
    }
}

/* Function to transpose a matrix */
double** transposeMatrix(double** mat, int n, int d) {
    int i, j;
    double** transposed;
    transposed = mallocMatrix(d, n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < d; j++) {
            transposed[j][i] = mat[i][j]; 
        }
    }
    return transposed;
}

/*function to multiply two matrices*/
double** multiplyMatrices(double** mat1, int rows1, int cols1, double** mat2, int rows2, int cols2) {
    int i;
    int j;
    int k;
    double** result;

    /* Check if the matrices can be multiplied */
    if (cols1 != rows2) {
        printAndExit();
    }

    /* Allocate memory for the result matrix */
    result = mallocMatrix(rows1, cols2);

    /* Multiply the two matrices */
    for (i = 0; i < rows1; i++) {
        for (j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (k = 0; k < cols1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return result;
}

double calculationPerCell (double h, double wh, double hhth){
    double result;
    result = h * (1 - BETA + BETA * (wh / hhth));
    return result;
}

double** calculation (double** H, double** W_H, double** H_HT_H, int H_n, int H_d){
    int i;
    int j;
    double** new_H;
    new_H = mallocMatrix(H_n, H_d);
    for(i = 0; i < H_n; i++){
        for(j = 0; j< H_d; j++){
            new_H[i][j] = calculationPerCell(H[i][j], W_H[i][j], H_HT_H[i][j]);
        }
    }
    return new_H;

}
/*update H matrix*/

void updateH(double** W, double** old_H, int W_n, int old_H_n, int old_H_d, double **new_H){
    double** H_transpose;
    double** W_H;
    double** H_HT;
    double** H_HT_H;
    double** result;

   H_transpose = transposeMatrix(old_H, old_H_n, old_H_d);
    W_H = multiplyMatrices(W, W_n, W_n, old_H, old_H_n, old_H_d);
    H_HT = multiplyMatrices(old_H, old_H_n, old_H_d, H_transpose, old_H_d, old_H_n);
    H_HT_H = multiplyMatrices(H_HT, old_H_n, old_H_n, old_H, old_H_n, old_H_d);
    result = calculation(old_H, W_H, H_HT_H, old_H_n, old_H_d);
    copyMatrix(result, new_H, old_H_n, old_H_d);

    freeMatrix(H_transpose, old_H_d);
    freeMatrix(W_H, W_n);
    freeMatrix(H_HT, old_H_n);
    freeMatrix(H_HT_H, old_H_n);
    freeMatrix(result, old_H_n);
}

double euclideanDistance(double** mat1, double** mat2, int rows, int cols) {
    double sum = 0.0;
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            double diff = mat1[i][j] - mat2[i][j];
            sum += diff * diff;
        }
    }
    return sum;
}


/* function that convergence H*/
double** convergnceH (double** W, double** old_H, int W_n, int old_H_n, int old_H_d){
    int countIter;
    double** new_H;
    double convergnce;
    countIter = 0;
    convergnce = 1.0;
    new_H = mallocMatrix(old_H_n, old_H_d);
    copyMatrix(old_H, new_H, old_H_n, old_H_d);
    while (countIter < 300 && convergnce > EPSILON){
        updateH(W, new_H, W_n, old_H_n, old_H_d, new_H);
        convergnce = euclideanDistance(new_H, old_H, old_H_n, old_H_d);
        countIter++;
        copyMatrix(new_H, old_H, old_H_n, old_H_d);
    }  
    return new_H;      
}


int main(int argc, char* argv[]) {
    char* goal;
    char* file_name;

    /* Parse command-line arguments */
    if (argc != 3) {
        printAndExit();
    }

    if (stdin == NULL) {
        printAndExit();
    }
    goal = argv[1];
    file_name = argv[2];

    executed_goal(file_name, goal);

    return 0;
}
