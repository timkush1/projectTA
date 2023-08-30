#define PY_SSIZE_T_CLEAN
#include <Python.h> /*can ignore*/
#include <stdlib.h>
#include <math.h>
#include "symnmf.h"
// #include <numpy/arrayobject.h>

// Function prototype declaration
static double **convertPyToArray(PyObject *centerList, int N, int D);

static double **convertPyToArray(PyObject *centerList, int N, int D) {
    double **clusters = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        PyObject *vector = PyList_GetItem(centerList, i);
        clusters[i] = (double *)malloc(D * sizeof(double));
        for (int j = 0; j < D; j++) {
            PyObject *element = PyList_GetItem(vector, j);
            clusters[i][j] = PyFloat_AsDouble(element);
        }
    }
    return clusters;
}

static PyObject *fit(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) {
        return NULL;
    }

    double **clusters = convertPyToArray(centerList, N, D);
    // ... Rest of your fit function code ...


    printf("sasiaa \n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < D; j++) {
            printf("%.4f", clusters[i][j]);
            if (j < D - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }





    for (int i = 0; i < N; i++) {
        free(clusters[i]);
    }
    free(clusters);

    Py_RETURN_NONE;
}

static PyObject *sym(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) {
        return NULL;
    }

    double **clusters = convertPyToArray(centerList, N, D);
    // ... Rest of your sym function code ...

    for (int i = 0; i < N; i++) {
        free(clusters[i]);
    }
    free(clusters);

    Py_RETURN_NONE;
}

static PyObject *ddg(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) {
        return NULL;
    }

    double **clusters = convertPyToArray(centerList, N, D);
    // ... Rest of your ddg function code ...

    for (int i = 0; i < N; i++) {
        free(clusters[i]);
    }
    free(clusters);

    Py_RETURN_NONE;
}

static PyObject *norm(PyObject *self, PyObject *args) {
    PyObject* centerList;
    int N;
    int D;
    if (!PyArg_ParseTuple(args, "Oii", &centerList, &D, &N)) {
        return NULL;
    }

    double **clusters = convertPyToArray(centerList, N, D);
    // ... Rest of your norm function code ...

    for (int i = 0; i < N; i++) {
        free(clusters[i]);
    }
    free(clusters);

    Py_RETURN_NONE;
}

static PyMethodDef fit_methods[] = {
    {"fit", (PyCFunction)fit, METH_VARARGS, PyDoc_STR("The fit function receives as input the following arguments:")},
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
    fit_methods
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
