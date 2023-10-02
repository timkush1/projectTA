#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "symnmf.h"
const char* GENERAL_ERROR = "An Error Has Occurred";

double** createDdg(double** arr, int n,int d); /* create diagonal matrix*/
double rowSum(double* arr, int d); /* calculate sum of one row */
double** createNorm(double** A, double** D, int n); /* create the W(normalized) matrix */
void matrixMulti(double** first, double** second, double** result, int n); /* updating "result matrix" to be matrix multiplication of first*second */
double** createSim(double** X,double** A,int n, int d) ;
double getSquaredDistance(double *pointA, double *pointB, int D);   /* return distance between two points */
void printMatrix(double** x, int n);
double **read_file(FILE* input_file, int* dim_pointer, int* count_lines_ptr);
int find_dim(FILE* input_file);
double** extract_data_points(FILE* input_file, int d, int n);
int lines_counter(FILE *file);
void parse_to_double(char* str, double* values, int d);

int i;
int sum;
int k;
int z;

double** createSim(double** X,double** A,int n, int d) /*create sym matrix*/
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
     /* update A as the sym matrix*/
}

double getSquaredDistance(double *pointA, double *pointB, int D)  /*return distance between two points */
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
    if (d>3)
    {
        
    }
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
                D[i][i] = rowSum(arr[i], n); /* Pass 'd' as an argument here*/
            } 
        }
    }
    return D;
}

double rowSum(double* arr, int n)/*calculate sum of one row*/
 {
    int i;
    double sum = 0;
    for (i = 0; i < n; i++)
    {
        sum = sum + arr[i];
    }
    return sum;
}

double** createNorm(double** A, double** D, int n)/* create the W(normalized) matrix*/
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
    matrixMulti(W, D, result, n);

    /* Free the memory allocated for W */
    for (i = 0; i < n; i++) {
        free(W[i]);
    }
    free(W);
    
    return result;
}

void matrixMulti(double** first, double** second, double** result, int n)/* updating "result matrix" to be matrix multiplication of first*second*/
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


/*
 * Uses utility functions to determine the dimensionality of the data and the number of data points.
 * It then reads and parses the data points from the file.
 */
double **read_file(FILE* input_file, int* dim_pointer, int* count_lines_ptr) {
    double **points = NULL;
    *count_lines_ptr = lines_counter(input_file);
    if(*count_lines_ptr <0) {
        perror("Failed to count lines number");
        return NULL;
    }
    *dim_pointer = find_dim(input_file);

    points = extract_data_points(input_file, *dim_pointer,  *count_lines_ptr);
    return points;
}

int lines_counter(FILE *file) {
    int count = 0;
    char ch1 = ' ';
    char ch2 = ' ';
    if (!file) {
        return -1; /* ERROR INDICATOR*/
    }
    while (ch1 != EOF) {
        ch2 = ch1;
        ch1 = fgetc(file);
        if (ch1 == '\n') {
            if(ch2 >= '0' || ch2 <= '9')
                count++;
        }
    }

    if(ch2 != '\n')
        count++;
    
    /*restore original file position*/
    rewind(file);
    return count;
}

/*
 * This function reads the first line of the input file to determine
 * the number of dimensions by counting the number of commas.
 */
int find_dim(FILE* input_file) {
    int commas = 1;
    char chari;

    while ((chari = fgetc(input_file)) != EOF && chari != '\n') {
        if (chari == ',') {
            commas++;
        }
    }
    rewind(input_file);
    return commas;
}

/*
 * Reads the input file line by line, splits each line into a list of doubles,
 * and appends each list to a dynamically allocated array.
 */
double** extract_data_points(FILE* input_file, int d, int n) {
    double **points = malloc(n* sizeof(double*));
    double* values = NULL;
    char *buffer = malloc(d*30);
    int i,j;

    memset(buffer,0,d*30);
    for(i=0; i<n; i++) {
        if(fgets(buffer, d*30, input_file) == NULL){
            free(buffer);
            for (j = 0; j < i; j++) {
                free(points[j]);
            }
            free(points);
            return NULL;
        }
        values = malloc(d * sizeof(double));
        parse_to_double(buffer, values, d);

        points[i] = values;
        memset(buffer,0,d*30);
    }
    free(buffer);
    return points;
}
void parse_to_double(char* str, double* values, int d) {
    int index;
    int i;
    char* current_double_pointer;
    char delim;

    delim = ',';

    index = 0;
    current_double_pointer = str;
    for (i = 0; i < d; i++) {
        while (str[index] != delim && str[index] != '\0') {
            index++;
        }
        str[index] = '\0';
        values[i] = strtod(current_double_pointer, NULL);

        current_double_pointer = &str[index + 1];
        index++;
    }
}

int main(int argc, char** argv) {
    double **points = NULL;
    int d = 0;
    int n = 0;
    int i;
    char *file_name = argv[2];
    char *matrix_type = argv[1];
    double **matrix;
    double **tempMatrix = NULL;
    double **tempMatrix2 = NULL;
    FILE *file = fopen(file_name, "r");
    if (argc != 3) {
        perror(GENERAL_ERROR);
        return 1;
    }
    
    if (!file) {
        perror(GENERAL_ERROR);
        return 1;
    }

    points = read_file(file, &d, &n);
    if (!points) {
        perror(GENERAL_ERROR);
        return 1;
    }

    if(strcmp(matrix_type, "sym") == 0) {
    
        matrix = malloc(n * sizeof(double*));
        if (matrix == NULL) {
            perror(GENERAL_ERROR);
            return 1;
        }
        for(i=0; i<n; i++) {
            matrix[i] = malloc(n*sizeof(double));
            if (matrix[i] == NULL) {
                perror(GENERAL_ERROR);
                return 1;
            }
        }
            createSim(points, matrix, n, d);
    } else
    {
            tempMatrix = malloc(n * sizeof(double*));
            if (tempMatrix == NULL) {
            perror(GENERAL_ERROR);
            return 1;
            }


            for(i=0; i<n; i++) { 
            tempMatrix[i] = malloc(n*sizeof(double));
            if (tempMatrix[i] == NULL) {
                perror(GENERAL_ERROR);
                return 1;
            }
            }

            if(strcmp(matrix_type, "ddg") == 0) {
                createSim(points, tempMatrix, n, d);
                matrix = createDdg(tempMatrix, n, d);
            } else if(strcmp(matrix_type, "norm") == 0){
                createSim(points, tempMatrix, n, d);
                tempMatrix2 = createDdg(tempMatrix, n, d);
                matrix = createNorm(tempMatrix,tempMatrix2, n);
            } else{
                perror(GENERAL_ERROR);
                return 1;
            }
    }
    


    printMatrix(matrix,n); 
    
    for (i = 0; i < n; i++) {
        free(points[i]);
    }
    free(points);

    for(i=0; i<n;i++) {
        free(matrix[i]);
    }
    free(matrix);

    if(strcmp(matrix_type, "sym") != 0) {

        for(i=0; i<n;i++) {
        free(tempMatrix[i]);
    }
    free(tempMatrix);
    }

    if(strcmp(matrix_type, "norm") == 0) {

        for(i=0; i<n;i++) {
        free(tempMatrix2[i]);
    }
    free(tempMatrix2);

    }

    fclose(file);
    return 0;
}
void printMatrix(double** x, int n){
    int i;
    int j;
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++) {
            printf("%.4f", x[i][j]);
            if(j!=n-1){
                printf(",");
            }
        }
            printf("\n");
    }

}
