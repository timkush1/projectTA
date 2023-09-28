#define PY_SSIZE_T_CLEAN
#include <Python.h> /*can ignore*/
#include <stdlib.h>
#include <math.h>
#include <stdbool.h> // Include this for boolean support
#include "symnmf.h"
// #include <numpy/arrayobject.h>

static double **convertPyToArray(PyObject *centerList, int N, int D);
PyObject* convertArrayToPy(double** array, int rows, int cols);
static void transpose(double **H, int n, int k, double **HT);
static void matrix_multiply(double **matrix1, double **matrix2, int m1, int k1, int k2, int m2, double **mult);
double frobenius_norm(double **matrix1, double **matrix2, int n, int k);


static double **convertPyToArray(PyObject *centerList, int N, int D) 
{
    double **X = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        PyObject *vector = PyList_GetItem(centerList, i);
        X[i] = (double *)malloc(D * sizeof(double));
        for (int j = 0; j < D; j++) {
            PyObject *element = PyList_GetItem(vector, j);
            X[i][j] = PyFloat_AsDouble(element);
        }
    }
    return X;
}

PyObject *convertArrayToPy(double **finalC, int K, int D)
{
    PyObject *outer_list = PyList_New(K);
    int i;
    int j;
    for (i = 0; i < K; i++)
    {
        PyObject *inner_list = PyList_New(D);
        for (j = 0; j < D; j++)
        {
            PyObject *py_double = PyFloat_FromDouble(finalC[i][j]);
            PyList_SetItem(inner_list, j, py_double); // Note: PyList_SetItem steals a reference to item.
        }
        PyList_SetItem(outer_list, i, inner_list);
    }

    return outer_list;
}


static void transpose(double **H, int n, int k, double **HT) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < k; j++) {
            HT[j][i] = H[i][j];
        }
    }
}

// Function to perform matrix multiplication for matrices
static void matrix_multiply(double **matrix1, double **matrix2, int m1, int k1, int k2, int m2, double **mult) {
    if (k1 != k2) {
        // Error handling since the matrices are incompatible for multiplication
        fprintf(stderr, "An Error Has Occurred");
        return;
    }
    for(int i = 0; i < m1; i++)
        for(int j = 0; j < m2; j++) {
            mult[i][j] = 0;
            for(int l = 0; l < k1; l++)
                mult[i][j] += matrix1[i][l] * matrix2[l][j]; 
        }
}

double frobenius_norm(double **matrix1, double **matrix2, int n, int k) {
    double sum_of_squares = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            double diff = matrix1[i][j] - matrix2[i][j];
            sum_of_squares += diff * diff;
        }
    }
    return sqrt(sum_of_squares);
}


static PyObject *symnmf(PyObject *self, PyObject *args) {
    PyObject *pyW, *pyH;
    int n, d,k;
    if (!PyArg_ParseTuple(args, "OOiii", &pyW, &pyH, &d, &n, &k)) {
        return NULL;
    }

    double **W = convertPyToArray(pyW,n,n);
    // printMatrix(W,n,n);
    double **H = convertPyToArray(pyH,n,k);
    // printMatrix(H,n,k);
    // update_H(W, H, n);
    
    double beta = 0.5;
    double epsilon = 1e-4;
    bool converged = false;
    int max_iterations = 300;
    int iteration = 0;
    
    double **HT = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < k; i++) {
        HT[i] = (double *)malloc(n * sizeof(double));
    }

    double **WH = (double **)malloc(n * sizeof(double *));
    double **HH = (double **)malloc(n * sizeof(double *));
    double **HHH = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        WH[i] = (double *)malloc(k * sizeof(double));
        HH[i] = (double *)malloc(n * sizeof(double));
        HHH[i] = (double *)malloc(k * sizeof(double));
    }
    
    // Free the allocated memory
     while (!converged && iteration < max_iterations) {
        
        transpose(H,n,k,HT);//TRANSPOSE HT
        matrix_multiply(W, H, n, n, n, k, WH);  // WH = W.H
        matrix_multiply(H, HT, n, k, k, n, HH);  // HH = H.HT
        matrix_multiply(HH, H, n, n, n, k, HHH);  // HHH = HH.H

        double **H_new = (double **)malloc(n * sizeof(double *));
        for (int i = 0; i < n; i++) {
            H_new[i] = (double *)malloc(k * sizeof(double));
            for(int j = 0; j < k; j++) {
                H_new[i][j] = H[i][j] * (1-beta + beta * (WH[i][j] / HHH[i][j]));
            }
        }

       
        if ((frobenius_norm(H_new, H, n , k)*frobenius_norm(H_new, H, n , k)) < epsilon) {
            converged = true;
        }

        // Swap H and H_new
        double **temp = H;
        H = H_new;
        H_new = temp;
        for (int i = 0; i < n; i++) {
            free(H_new[i]);
        }
        free(H_new);  // Since H_new points to old H now


        if (iteration == 20)
        {
            // printMatrix(H,n,k);
        }
        iteration++;
    }
    // printMatrix(H,n,k);
    PyObject* py_H = convertArrayToPy(H,n,k);

    // Free the allocated memory
    for (int i = 0; i < n; i++) {
        free(W[i]);
        free(H[i]);
        free(WH[i]);
        free(HH[i]);
        free(HHH[i]);
    }
    free(W);
    free(H);
    free(WH);
    free(HH);
    free(HHH);

    for (int i = 0; i < k; i++) {
        free(HT[i]);
    }
    free(HT);

    return py_H;;
}


static PyObject *norm(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    double** diag;
    double** A;
    double** norm;
    int i;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) //checking that function gets the rigth arguments 
    {
        return NULL;
    }
    double **X = convertPyToArray(centerList, N, D);//convert X to 2D array

    
    A = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    // diag = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    // norm = (double**)malloc(N * sizeof(double*));   
    for (i = 0; i < N; i++)
    {
        A[i] = (double*)malloc(N * sizeof(double));
        // diag[i] = (double*)malloc(N * sizeof(double));
        // norm[i] = (double*)malloc(N * sizeof(double));
    }
    A = createSim(X,A,N,D);
    diag=createDdg(A,N,N);
    norm = createNorm(A,diag,N);
    PyObject* py_H = convertArrayToPy(norm,N,N);


    // Free the allocated memory for X
    for (i = 0; i < N; i++) {
        free(X[i]);
    }
    free(X);

    // Free the allocated memory for A
    for (i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);

    // Free the allocated memory for diag
    for (i = 0; i < N; i++) {
        free(diag[i]);
    }
    free(diag);

    // Free the allocated memory for norm
    for (i = 0; i < N; i++) {
        free(norm[i]);
    }
    free(norm);

    return py_H;
}

static PyObject *sym(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    double** A;
    int i;
    
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) 
    {
        return NULL;
    }
    
    double **X = convertPyToArray(centerList, N, D);

    A = (double**)malloc(N * sizeof(double*));
    for (i = 0; i < N; i++)
    {
        A[i] = (double*)malloc(N * sizeof(double));
        for(int j=0; j<N; j++)
        {
            A[i][j] = 0;
        }
    }

    A = createSim(X, A, N, D);
    PyObject* py_H = convertArrayToPy(A, N, N);

    // Free X
    for (i = 0; i < N; i++) 
    {
        free(X[i]);
    }
    free(X);

    // Free A
    for (i = 0; i < N; i++)
    {
        free(A[i]);
    }
    free(A);

    return py_H;
}


static PyObject *ddg(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    double** A;
    double** diag;
    int i;
    
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N))//checking that function gets the rigth arguments 
    {
        return NULL;
    }
    double **X = convertPyToArray(centerList, N, D);//convert X to 2D array
    A = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    // diag = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    for (i = 0; i < N; i++)
    {
        A[i] = (double*)malloc(N * sizeof(double));
        // diag[i] = (double*)malloc(N * sizeof(double));
    }
    if(createDdg(X,N,D) == NULL) //check that there arent errors on the call
    {
        /*yErr_SetString(PyExc_RuntimeError, GENERAL_ERROR);*/
        return NULL;
    }
    A = createSim(X,A,N,D);
    diag=createDdg(A,N,N);
    // printMatrix(diag,N,N);//print the matrix
    PyObject* py_H = convertArrayToPy(diag,N,N);
    for (i = 0; i < N; i++) 
    {
        free(X[i]);
        free(A[i]);
        free(diag[i]);
    }
    free(X);
    free(A);
    free(diag);
    return py_H;
}


static PyMethodDef symnmf_methods[] = {
    {"symnmf", (PyCFunction)symnmf, METH_VARARGS, PyDoc_STR("The symnmf function receives as input the following arguments:")},
    {"ddg", (PyCFunction)ddg, METH_VARARGS, PyDoc_STR("Docstring for ddg function")},
    {"sym", (PyCFunction)sym, METH_VARARGS, PyDoc_STR("Docstring for sym function")},
    {"norm", (PyCFunction)norm, METH_VARARGS, PyDoc_STR("Docstring for norm function")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf",
    NULL,
    -1,
    symnmf_methods
};

PyMODINIT_FUNC PyInit_symnmf(void){
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m)
    {
        PyErr_SetString(PyExc_RuntimeError, "An error occurred");
        return NULL;
    }
    return m;
}
