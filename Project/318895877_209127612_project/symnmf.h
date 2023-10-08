#ifndef SYMNMF_H
#define SYMNMF_H

void sym(double** X, int n, int d, double** A);
void ddg(double** A, int n, double** D);
void norm(double** A, double** D, int n, double** W);
void printMatrix(double** matrix, int n, int d);
void printAndExit(void);
void freeMatrix(double** matrix, int n);
struct fileData read_file(char* file_name);
void executed_goal(char* file_name, char* goal);
int main(int argc, char* argv[]);
double** mallocMatrix(int n, int d);
double** transposeMatrix(double** mat, int n, int d);
double** multiplyMatrices(double** mat1, int rows1, int cols1, double** mat2, int rows2, int cols2);
double calculationPerCell (double h, double wh, double hhth);
double** calculation (double** H, double** W_H, double** H_HT_H, int H_n, int H_d);
void updateH(double** W, double** old_H, int W_n, int old_H_n, int old_H_d, double **new_H);
double euclideanDistance(double** mat1, double** mat2, int rows, int cols);
double** convergnceH (double** W, double** old_H, int W_n, int old_H_n, int old_H_d);

#endif
