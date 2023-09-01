#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "symnmf.h"

double** createDdg(double** arr, int n,int d); // create diagonal matrix
double rowSum(double* arr, int d); // calculate sum of one row
double** createNorm(double** A, double** D, int n); // create the W(normalized) matrix
void matrixMulti(double** first, double** second, double** result, int n); // updating "result matrix" to be matrix multiplication of first*second
double** createSim(double** X,double** A,int n, int d) ;
double getSquaredDistance(double *pointA, double *pointB, int D);   //return distance between two points */
void printMatrix(double** x, int n,int d);

int i;
int sum;
int k;
int z;

double** createSim(double** X,double** A,int n, int d) //create sym matrix
{
    int i;
    int j;
    double z;
    for (i = 0; i < n; i++) 
    {
        for (j = 0; j < n; j++)
         {
            if (i == j) 
            { 
                A[i][j] = 0;
            }
             else 
            {
                z = getSquaredDistance(&X[i][0], &X[j][0], d);
                A[i][j] = exp(z*(-0.5)); 
            }
        }
    }
    return A;
     // update A as the sym matrix
}

double getSquaredDistance(double *pointA, double *pointB, int D)  //return distance between two points */
{
    int k;
    double distance = 0;
    for (k = 0; k< D; k++) 
    {
        distance += (pointA[k]-pointB[k])*(pointA[k]-pointB[k]);
    }
    return distance;
}

double** createDdg(double** arr, int n,int d) 
{
    int i;
    int j;
    double** D = (double**)malloc(n * sizeof(double*));
    for (i = 0; i < n; i++) 
    {
        D[i] = (double*)malloc(n * sizeof(double));
        for(j=0;j<n;j++)
        {
            if(i!=j)
            {
                D[i][j] = 0;
            }
            else
            {
                D[i][i] = rowSum(arr[i], n); // Pass 'd' as an argument here
            } 
        }
    }
    return D;
}

double rowSum(double* arr, int n)// calculate sum of one row
 {
    int i;
    double sum = 0;
    for (i = 0; i < n; i++)
    {
        sum = sum + arr[i];
    }
    return sum;
}

double** createNorm(double** A, double** D, int n)// create the W(normalized) matrix
 {
    int i;
    int j;
    double** W = (double**)malloc(n * sizeof(double*));
    double** result = (double**)malloc(n * sizeof(double*));
    for (i = 0; i < n; i++)
    {
        W[i] = (double*)malloc(n * sizeof(double));
        result[i] = (double*)malloc(n * sizeof(double));
        D[i][i] = pow(D[i][i], -0.5);
        for (j = 0; j < n; j++)
        {
            result[i][j]=0;
            W[i][j]=0;
        }
    }
    
    matrixMulti(D, A, W, n);
    //printMatrix(W,n,n);
    matrixMulti(W, D, result, n);
    return result;
}

void matrixMulti(double** first, double** second, double** result, int n)// updating "result matrix" to be matrix multiplication of first*second
 {
    int i;
    int j;
    int k;
    for (i = 0; i < n; i++) 
    {
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < n; k++) 
            {
                result[i][j] += first[i][k] * second[k][j];
            }
        }
    }
}
void printMatrix(double** x, int n,int d)
{
    int i;
    int j;
    for (i = 0; i < n; i++) 
    {
        for (j = 0; j < d; j++) 
        {
            printf("%.4f,", x[i][j]); // Print each element followed by a coma
        }
         printf("\n"); // Print a new line after each row
    }
}