#define PY_SSIZE_T_CLEAN
#include <Python.h> /*can ignore*/
#include <stdlib.h>
#include <math.h>
#include "symnmf.h"
// #include <numpy/arrayobject.h>

// Function prototype declaration
static double **convertPyToArray(PyObject *centerList, int N, int D);
PyObject* convertArrayToPy(double** array, int rows, int cols);

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

static PyObject *symnmf(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    double** diag;
    double** A;
    double** norm;
    int i;
    int j;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) //checking that function gets the rigth arguments 
    {
        return NULL;
    }
    double **X = convertPyToArray(centerList, N, D);//convert X to 2D array

    
    A = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    diag = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    norm = (double**)malloc(N * sizeof(double*));   
    for (i = 0; i < N; i++)
    {
        A[i] = (double*)malloc(N * sizeof(double));
        diag[i] = (double*)malloc(N * sizeof(double));
        norm[i] = (double*)malloc(N * sizeof(double));
    }
    A = createSim(X,A,N,D);
    diag=createDdg(A,N,N);
    norm = createNorm(A,diag,N);
    PyObject* py_H = convertArrayToPy(norm,N,N);


    for (i = 0; i < N; i++) {
        free(X[i]);
    }
    free(X);

    return py_H;
}
static PyObject *norm(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    double** diag;
    double** A;
    double** norm;
    int i;
    int j;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) //checking that function gets the rigth arguments 
    {
        return NULL;
    }
    double **X = convertPyToArray(centerList, N, D);//convert X to 2D array

    
    A = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    diag = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    norm = (double**)malloc(N * sizeof(double*));   
    for (i = 0; i < N; i++)
    {
        A[i] = (double*)malloc(N * sizeof(double));
        diag[i] = (double*)malloc(N * sizeof(double));
        norm[i] = (double*)malloc(N * sizeof(double));
    }
    A = createSim(X,A,N,D);
    diag=createDdg(A,N,N);
    norm = createNorm(A,diag,N);
    // printf("the norm \n");
    // printMatrix(norm,N,N);
    PyObject* py_H = convertArrayToPy(norm,N,N);


    for (i = 0; i < N; i++) {
        free(X[i]);
    }
    free(X);

    return py_H;
}

static PyObject *sym(PyObject *self, PyObject *args) 
{
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


static PyObject *ddg(PyObject *self, PyObject *args) 
{
    PyObject* centerList;
    int N;
    int D;
    double** diag;
    int i;
    
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N))//checking that function gets the rigth arguments 
    {
        return NULL;
    }
    double **X = convertPyToArray(centerList, N, D);//convert X to 2D array
    diag = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
    for (i = 0; i < N; i++)
    {
        diag[i] = (double*)malloc(N * sizeof(double));
    }
    if(createDdg(X,N,D) == NULL) //check that there arent errors on the call
    {
        /*yErr_SetString(PyExc_RuntimeError, GENERAL_ERROR);*/
        return NULL;
    }
    diag=createDdg(X,N,D);//call the function
    // printMatrix(diag,N,N);//print the matrix
    PyObject* py_H = convertArrayToPy(diag,N,N);
    for (int i = 0; i < N; i++) 
    {
        free(X[i]);
    }
    free(X);
    return py_H;
}

// static PyObject *norm(PyObject *self, PyObject *args) {
//     PyObject* centerList;
//     int N;
//     int D;
//     double** norm;
//     int i;
//     double** A;
//     double** diag;
//     if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) 
//     {
//         return NULL;
//     }
    
//     double **X = convertPyToArray(centerList, N, D);//convert X to 2D array
//     norm = (double**)malloc(N * sizeof(double*));//allocate memory for the new matrix
//     A = (double**)malloc(N * sizeof(double*));
//     diag = (double**)malloc(N * sizeof(double*));
//     for (i = 0; i < N; i++)
//     {
//         norm[i] = (double*)malloc(N * sizeof(double));
//         A[i] = (double*)malloc(N * sizeof(double));
//         diag[i] = (double*)malloc(N * sizeof(double));
//     }
//     A = createSim(X,A,N,D);
//     diag = createDdg(X,N,D);
//     norm = createNorm(X,diag,N);
//     printMatrix(norm,N,D);
//     for (int i = 0; i < N; i++) {
//         free(X[i]);
//     }
//     free(X);
   
//     Py_RETURN_NONE;
// }

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

PyMODINIT_FUNC PyInit_symnmf(void)
{
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m)
    {
        PyErr_SetString(PyExc_RuntimeError, "An error occurred");
        return NULL;
    }
    return m;
}
