#define PY_SSIZE_T_CLEAN
#include <Python.h> /*can ignore*/
#include <stdlib.h>
#include <math.h>
#include "symnmf.h"
#include <numpy/arrayobject.h>

static PyObject *fit(PyObject *self, PyObject *args) {
    // Parse arguments and convert to C types

    PyObject* centerList;
    int N;
    int D;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) {
        return NULL;
    }


    double **clusters = (double **)malloc(N * sizeof(double *)); // Allocate memory for the clusters array
    for (int i = 0; i < N; i++)                                  // Iterate over each vector in the centerList
    {
        PyObject *vector = PyList_GetItem(centerList, i);   // Get the current vector
        clusters[i] = (double *)malloc(D * sizeof(double)); // Allocate memory for the current cluster
        for (int j = 0; j < D; j++)                         // Iterate over each element in the vector
        {
            PyObject *element = PyList_GetItem(vector, j); // Get the current element
            clusters[i][j] = PyFloat_AsDouble(element);    // Convert the element to a double and assign it to the cluster
        }
    }
    // printf("sasiaa \n");
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < D; j++) {
    //         printf("%.4f", clusters[i][j]);
    //         if (j < D - 1) {
    //             printf(", ");
    //         }
    //     }
    //     printf("\n");
    // }

    

    // Free memory for the clusters array
    for (int i = 0; i < N; i++) {
        free(clusters[i]);
    }
    free(clusters);

    Py_RETURN_NONE;
}




/* Create wrapper functions for the fit function */
/* &centerList, &currFile, &K, &D, &N, &iter, &epsilon*/
static PyMethodDef fit_methods[] = {
    {"fit",            /* the Python method name that will be used */
     (PyCFunction)fit, /* the C-function that implements the Python function and returns static PyObject*  */
     METH_VARARGS,     /* flags indicating parameters accepted for this function */
     PyDoc_STR("The fit function receives as input the following arguments:")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}                             /* The last entry must be all NULL as shown to act as a sentinel. Python looks for this
                                                       * entry to know that all the functions for the module have been defined. */
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf", /* name of module */
    NULL,       /* module documentation, may be NULL */
    -1,         /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    fit_methods /* the PyMethodDef array from before containing the methods of the extension */
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
