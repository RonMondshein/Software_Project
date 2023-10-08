#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double eculidean_distance(double *p1, double *p2, int d);
int argmin(double *x, double **centroid_array , int k, int d);
double** kmeans (double** n_array, double** cluster, int k, int iter, double epsilon, int d, int num_of_lines);
static PyObject* fit (PyObject *self, PyObject *args);

double eculidean_distance(double *p1, double *p2, int d){
    double sum;
    int i;
    sum =0;
    for (i = 0; i < d; i++){
        sum += pow((p1[i] - p2[i]), 2.0);
    }
    return sqrt(sum);
}

int argmin(double *x, double **centroid_array , int k, int d){

    double min_distance = eculidean_distance(x, centroid_array[0], d);
    int min_index =  0;
    int i;

    for ( i = 1; i < k; i++){
        double distance = eculidean_distance(x, centroid_array[i], d);
        if (distance < min_distance){
            min_distance = distance;
            min_index = i;
        }
    }
    return min_index;
}

double** kmeans (double** n_array, double** cluster, int k, int iter, double epsilon, int d, int num_of_lines) {
    int *count_cluster;
    int i;
    int j;
    int new_index;
    int is_converge;
    int num_of_iter;
    int min_index;
    double **new_centroid_array;
    double **old_centroid_array;
    double **total_cluster;
    double *x;
    double delta;

    new_index = 0;
    i=0;
    is_converge = 0;
    num_of_iter =  0;
    new_centroid_array = calloc(k, sizeof(double *));
    if (new_centroid_array == NULL) {
        return NULL;
    }
    old_centroid_array = calloc(k, sizeof(double *));
    if (old_centroid_array == NULL) {
        return NULL;
    }
    for (i = 0; i < k; i++)
    {
        old_centroid_array[i] = calloc(d, sizeof(double));
        if (old_centroid_array[i] == NULL) {
            return NULL;
        }
        new_centroid_array[i] = calloc(d, sizeof(double));
        if (new_centroid_array[i] == NULL) {
            return NULL;
        }
        for (j = 0; j < d; j++){
            old_centroid_array[i][j] = cluster[i][j];
            new_centroid_array[i][j] = 0.0;
        }
    }

    total_cluster = calloc(k, sizeof(double *));
    if (total_cluster == NULL) {
            return NULL;
    }
    count_cluster = calloc(k, sizeof(int));
    if (count_cluster == NULL) {
            return NULL;
    }
    for (i = 0; i < k; i++)
    {
        total_cluster[i] = calloc(d, sizeof(double));
    if (total_cluster[i] == NULL) {
            return NULL;
    }
        count_cluster[i] = 0;
    }

    while (is_converge == 0 && num_of_iter < iter)
    {
        for(i = 0; i < num_of_lines; i++){
            x = n_array[i];
            min_index = argmin(x, old_centroid_array, k, d);
            for(new_index = 0; new_index < d; new_index++){
                total_cluster[min_index][new_index] += x[new_index];
            }
            count_cluster[min_index] += 1;
        }
        
        num_of_iter += 1;
        
        for (j = 0; j < k; j++){
            for(i = 0; i < d; i++){
                new_centroid_array[j][i] = total_cluster[j][i] / count_cluster[j];
            }
        } 

        is_converge =  1;
        for (i = 0; i < k; i++){
            delta = eculidean_distance(new_centroid_array[i], old_centroid_array[i], d);
            if (delta >= epsilon){
                is_converge = 0;
                break;
            }
        }

        for(i = 0; i < k; i++){
            for(j = 0; j < d; j++){
                old_centroid_array[i][j] = new_centroid_array[i][j];
                total_cluster[i][j] = 0.0;
            }
            count_cluster[i] = 0;
        } 
    }

    for (i = 0; i < k; i++)
    {
        free(old_centroid_array[i]);
        free(total_cluster[i]);
        free(cluster[i]);
    }
    for (i = 0; i < num_of_lines; i++)
    {
        free(n_array[i]);
    }
    free(n_array);
    free(cluster);
    free(old_centroid_array);
    free(total_cluster);
    free(count_cluster);

    return new_centroid_array;
        
}

static PyObject* fit (PyObject *self, PyObject *args)
{
    PyObject *n_array_py;
    PyObject *cluster_py;
    PyObject *vector;
    PyObject *cord;
    PyObject *final_centroids;
    double** n_array;
    double** cluster; 
    double** new_centroid_array;
    int k;
    int iter;
    double epsilon; 
    int d;
    int num_of_lines;
    int i;
    int j;

    if(!PyArg_ParseTuple(args, "OOiidii", &n_array_py, &cluster_py, &k, &iter, &epsilon, &d, &num_of_lines))
    {
        return NULL;
    }
    n_array = malloc(num_of_lines * sizeof(double *));  
    if (n_array == NULL) {
        return NULL;
    }
    

    for(i = 0; i < num_of_lines; i++){
        n_array[i] = malloc(d * sizeof(double));
        if (n_array[i] == NULL){
            return NULL;
        }
        
        vector = PyList_GetItem(n_array_py, i);
        for(j = 0; j < d; j++){
            cord = PyList_GetItem(vector, j);
            n_array[i][j] = PyFloat_AsDouble(cord);
        }
    }   
    cluster = calloc(k, sizeof(double *));
    if (cluster == NULL) {
        return NULL;
    }
    for (i = 0; i < k; i++)
    {
        cluster[i] = calloc(d, sizeof(double));
        if (cluster[i] == NULL) {
            return NULL;
        }
        vector = PyList_GetItem(cluster_py, i);
        for (j = 0; j < d; j++){
            cord = PyList_GetItem(vector, j);
            cluster[i][j] = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(cluster_py, i), j));
        }
        
    }
    new_centroid_array =  kmeans(n_array, cluster, k, iter, epsilon, d, num_of_lines);
    if  (new_centroid_array == NULL) {
        return NULL;
    }
    final_centroids =  PyList_New(k);
    for (i = 0; i < k; i++){
        vector = PyList_New(d);
        for (j = 0; j < d; j++){
            
            cord = PyFloat_FromDouble(new_centroid_array[i][j]);
            PyList_SetItem(vector, j, cord);
        }
        PyList_SetItem(final_centroids, i, vector);
    }
    for (i = 0; i < k; i++) {
        free(new_centroid_array[i]);
    }
    free(new_centroid_array);

    return Py_BuildValue("O",final_centroids);
}

static PyMethodDef kmeansMethods[] = {
    {"fit", 
    (PyCFunction)fit, 
    METH_VARARGS, 
    PyDoc_STR("Excute kmeans clusters algorithm. RETURN: the centroids of the clusters.")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    NULL,
    -1,
    kmeansMethods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject  *m;
    m = PyModule_Create(&kmeansmodule);
    if (m == NULL){
        return NULL;
    }
    return m;
}
