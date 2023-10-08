#define PY_SSIZE_T_CLEAN 
#include <Python.h>
#include <math.h>
#include "symnmf.h"
#define EPSILON 1e-4
#define MAX_ITER 300
#define BETA 0.5

void arrayDimensions(PyObject* array, int* n, int* d){
    PyObject *vector;
    *n = PyList_Size(array);
    vector = PyList_GetItem(array, 0);
    *d = PyList_Size(vector);
}
double** fromPyArrayToCArray(PyObject* pyArray, int n, int d){
    double** cArray;
    PyObject *vector;
    PyObject *cord;
    int i;
    int j;
    cArray = mallocMatrix(n, d);
    for (i = 0; i < n; i++) {
        vector = PyList_GetItem(pyArray, i);
        for (j = 0; j < d; j++){
            cord = PyList_GetItem(vector, j);
            cArray[i][j] = PyFloat_AsDouble(cord);
        }
    }
    return cArray;
}

PyObject* fromCArrayToPythonArray(double** arr, int n, int d) {
    PyObject* pyList;
    PyObject* pyRow;
    PyObject* pyFloat;
    int i;
    int j;
    pyList = PyList_New(n);
    if (pyList == NULL) {
        printAndExit();
    }
    for (i = 0; i < n; i++) {
        pyRow = PyList_New(d);
        if (pyRow == NULL) {
            printAndExit();
        }
        for (j = 0; j < d; j++) {
            pyFloat = PyFloat_FromDouble(arr[i][j]);
            if (pyFloat == NULL) {
                printAndExit();
            }
            PyList_SET_ITEM(pyRow, j, pyFloat);
        }
        PyList_SET_ITEM(pyList, i, pyRow);
    }
    return pyList;
}

double** symnmf(double** init_H, double** W, int W_n, int init_H_n, int init_H_d){
    return convergnceH(W, init_H, W_n, init_H_n, init_H_d);
}
static PyObject* fit_symnmf (PyObject *self, PyObject *args)
{
    PyObject *init_H_py;
    PyObject *W_py;
    PyObject *final_H;
    double** init_H;
    double** W;
    double** H;
    int init_H_n;
    int init_H_d;
    int W_n;
    int W_d;
    if(!PyArg_ParseTuple(args, "OO", &init_H_py, &W_py)){
        return NULL;
    }
    arrayDimensions(init_H_py, &init_H_n, &init_H_d);
    arrayDimensions(W_py, &W_n, &W_d);
    W = fromPyArrayToCArray(W_py, W_n, W_d);
    init_H = fromPyArrayToCArray(init_H_py, init_H_n, init_H_d);
    H = symnmf(init_H, W, W_n, init_H_n, init_H_d);    
    final_H = fromCArrayToPythonArray(H, init_H_n, init_H_d);
    freeMatrix(init_H, init_H_n);
    freeMatrix(W, W_n);
    freeMatrix(H, init_H_n);
    return Py_BuildValue("O",final_H);
}
static PyObject* fit_sym (PyObject *self, PyObject *args)
{
    PyObject *X_py;
    PyObject *final_A;
    double** X;
    double** A;
    int n;
    int d;

    if(!PyArg_ParseTuple(args, "O", &X_py))
    {
        return NULL;
    }
    arrayDimensions(X_py, &n, &d);
    X = fromPyArrayToCArray(X_py, n, d);
    A  = mallocMatrix(n, n);
    sym(X, n, d, A);
    final_A = fromCArrayToPythonArray(A, n, n);
    freeMatrix(X, n);
    freeMatrix(A, n);
    return Py_BuildValue("O",final_A);
}
static PyObject* fit_ddg (PyObject *self, PyObject *args)
{
    PyObject *X_py;
    PyObject *final_D;
    double** X;
    double** A;
    double** D;
    int n;
    int d;

    if(!PyArg_ParseTuple(args, "O", &X_py))
    {
        return NULL;
    }
    arrayDimensions(X_py, &n, &d);

    X = fromPyArrayToCArray(X_py, n, d);
    A = mallocMatrix(n, n);
    sym(X, n, d, A);
    D  = mallocMatrix(n, n);
    ddg(A, n, D);
    final_D = fromCArrayToPythonArray(D, n, n);
    freeMatrix(X, n);
    freeMatrix(D, n);
    freeMatrix(A, n);
    return Py_BuildValue("O",final_D);
}
static PyObject* fit_norm (PyObject *self, PyObject *args)
{
    PyObject *X_py;
    PyObject *final_W;
    double** X;
    double** A;
    double** D;
    double** W;
    int n;
    int d;

    if(!PyArg_ParseTuple(args, "O", &X_py))
    {
        return NULL;
    }
    arrayDimensions(X_py, &n, &d);

    X = fromPyArrayToCArray(X_py, n, d);
    A = mallocMatrix(n, n);
    sym(X, n, d, A);
    D = mallocMatrix(n, n);
    ddg(A, n, D);
    W  = mallocMatrix(n, n);
    norm(A, D, n, W);
    final_W = fromCArrayToPythonArray(W, n, n);
    freeMatrix(X, n);
    freeMatrix(W, n);
    freeMatrix(D, n);
    freeMatrix(A, n);
    return Py_BuildValue("O",final_W);
}

static PyMethodDef symnmfMethods[] = {
    {"fit_symnmf", (PyCFunction)fit_symnmf, METH_VARARGS, PyDoc_STR("Perform full the symNMF and output H.")},
    {"fit_sym", (PyCFunction)fit_sym, METH_VARARGS, PyDoc_STR("Calculate and output the similarity matrix.")},
    {"fit_ddg", (PyCFunction)fit_ddg, METH_VARARGS, PyDoc_STR("Calculate and output the Diagonal Degree Matrix.")},
    {"fit_norm", (PyCFunction)fit_norm, METH_VARARGS, PyDoc_STR("Calculate and output the normalized similarity matrix.")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "mysymnmf",
    NULL,
    -1,
    symnmfMethods
};

PyMODINIT_FUNC PyInit_mysymnmf(void)
{
    PyObject  *m;
    m = PyModule_Create(&symnmfmodule);
    if (m == NULL){
        return NULL;
    }
    return m;
}
