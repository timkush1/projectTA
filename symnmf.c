#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "symnmf.h"

double** createDdg(double** arr, int n, int d); // create diagonal matrix
double rowSum(double* arr, int d); // calculate sum of one row
double** createNorm(double** A, double** D, int n, int d); // create the W(normalized) matrix
void matrixMulti(double** first, double** second, double** result, int n, int d); // updating "result matrix" to be matrix multiplication of first*second
double** createSim(double** X, int n, int d); // create matrix A(similarity matrix)
double getSquaredDistance(double pointA, double *pointB, int D); / return distance between two points */
void printMatrix(double** x, int n, int d);

int i;
int sum;
int j;
int k;
int z;

double** createSim(double** X, int n, int d) 
{
    double** A = (double*)malloc(n * sizeof(double));
    for (i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        for (j = 0; j < n; j++)
         {
            if (i == j) 
            { // Use '==' instead of '=' for comparison
                A[i][j] = 0;
            }
             else 
            {
                z = getSquaredDistance(X[i], X[j], d);
                A[i][j] = exp(z*(-0.5)); // Add return statement here
            }
        }
    }
    return A; // Return the created matrix
}

double getSquaredDistance(double *pointA, double *pointB, int D) 
{
    double distance = 0;
    for (j = 0; j < D; j++) 
    {
        distance = distance + pow((pointA[j]-pointB[j]),2);
    }
    return distance;
}

double** createDdg(double** arr, int n, int d) {
    double** D = (double*)malloc(n * sizeof(double));
    for (i = 0; i < n; i++) {
        D[i] = (double*)malloc(d * sizeof(double));
        D[i][i] = rowSum(arr[i], d); // Pass 'd' as an argument here
    }
    return D;
}

double rowSum(double* arr, int d)
 {
    sum = 0;
    for (i = 0; i < d; i++)
    {
        sum = sum + arr[i];
    }
    return sum;
}

double** createNorm(double** A, double** D, int n, int d)
 {
    double** W = (double*)malloc(n * sizeof(double));
    for (i = 0; i < n; i++)
    {
        W[i] = (double*)malloc(d * sizeof(double));
        D[i][i] = pow(D[i][i], -0.5); // Correct the math function name
    }
    matrixMulti(D, A, W, n, d);
    matrixMulti(W, D, W, n, d);
    return W;
}

void matrixMulti(double** first, double** second, double** result, int n, int d) {
    for (i = 0; i < n; i++) 
    {
        for (j = 0; j < n; j++)
        {
            result[i][j] = 0; // Initialize result[i][j] to 0
            for (k = 0; k < n; k++) {
                result[i][j] += first[i][k] * second[k][j];
            }
        }
    }
}
void printMatrix(double** x, int n, int d)
{
   for (i = 0; i < n; i++) 
    {
        for (j = 0; j < d; j++) 
        {
            printf("%lf ", x[i][j]); // Print each element followed by a spac/e
                   }
         printf("\n"); // Print a new line after each row
 }
}