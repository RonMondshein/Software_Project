#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct cord cord;
typedef struct vector vector;

double eculidean_distance(double *p1, double *p2, int d);
int argmin(double *x, double **centroid_array , int k, int d);
double** create_n_Array(struct vector* head, int num_of_lines, int d);
void free_vectors(struct vector* head);

#define EPSILON 0.001

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

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

double** create_n_Array(struct vector* head, int num_of_lines, int d) {
    struct vector* current_vector;
    struct cord* current_cord;
    double** n_array;
    int i;
    int j;
    i = 0;
    j = 0;
    current_vector = head;
    current_cord =NULL;

    n_array = malloc(num_of_lines * sizeof(double*));
    if (n_array == NULL) {
        printf("An Error Has Occurred");
        exit(1);
    }

    current_vector = head;
    while (current_vector->next != NULL) {
        n_array[i] = malloc(d * sizeof(double));
        if (n_array[i] == NULL) {
            printf("An Error Has Occurred");
            exit(1);
        }

        current_cord = current_vector->cords;
        j = 0;
        while (current_cord != NULL) {
            n_array[i][j] = current_cord->value;
            current_cord = current_cord->next;
            j++;
        }

        current_vector = current_vector->next;
        i++;
    }

    return n_array;
}

void free_vectors(struct vector* head) {
    struct vector* current;
    struct vector* temp_vector;
    struct cord* current_cord;
    struct cord* temp_cord;
    current = head;
    current_cord = NULL;
    temp_vector = NULL;
    temp_cord = NULL;
    while (current->next != NULL) {
        temp_vector = current;
        current_cord = current->cords;
        while (current_cord != NULL) {
            temp_cord = current_cord;
            current_cord = current_cord->next;
            free(temp_cord);
        }
        current = current->next;
        free(temp_vector);
    }
    free(current);
}

int main(int argc, char *argv[]) {
    vector *head_vec;
    vector *curr_vec;
    cord *head_cord;
    cord *curr_cord;
    int *count_cluster;
    int k;
    int iter;
    int i;
    int j;
    int new_index;
    int vector_length;
    int num_of_lines;
    int d;
    int is_converge;
    int num_of_iter;
    int min_index;
    int have_error;
    double **n_array;
    double **new_centroid_array;
    double **old_centroid_array;
    double **total_cluster;
    double *x;
    double n;
    double delta;
    char c;

    new_index = 0;
    i=0;
    num_of_lines = 0;
    d=1;
    have_error = 0;
    is_converge = 0;
    num_of_iter = 0;
    vector_length = 1;


    if (argc == 3){
        k = atoi(argv[1]);
        i=0;
        while (argv[1][i] != '\0') {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                k=0;
                break;
            }
            i++;
        }
        iter = atoi(argv[2]);
        i=0;
        while (argv[2][i] != '\0') {
            if (argv[2][i] < '0' || argv[2][i] > '9') {
                iter=0;
                break;
            }
            i++;
        }
    }

    else if (argc == 2){
        k = atoi(argv[1]);
        i=0;
        while (argv[1][i] != '\0') {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                k=0;
                break;
            }
            i++;
        }
        iter = 200;
    }

    if (stdin == NULL) {
        printf("An Error Has Occurred");
        return 1;
    }

    head_cord = malloc(sizeof(struct cord));
    if (head_cord == NULL)
    {
        printf("An Error Has Occurred");
        return 1;
    }
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    if (head_vec == NULL)
    {
        printf("An Error Has Occurred");
        return 1;
    }
    curr_vec = head_vec;
    curr_vec->next = NULL;
    
    while (scanf("%lf%c", &n, &c) == 2)
    {

        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if (curr_vec->next == NULL)
            {
                printf("An Error Has Occurred");
                return 1;
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if (head_cord == NULL)
            {
                printf("An Error Has Occurred");
                return 1;
            }
            
            curr_cord = head_cord;
            curr_cord->next = NULL;
            num_of_lines++;
            

            if(d < vector_length)
                d = vector_length;
            
            vector_length = 0;

            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        if (curr_cord->next == NULL)
        {
            printf("An Error Has Occurred");
            return 1;
        }

        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        vector_length++;
    }
    
 
    if (k <= 1 || k >= num_of_lines){
        printf("Invalid number of clusters!");
        free_vectors(head_vec);
        free(head_cord);
        return 1;
        }
    if (iter <= 1 || iter >= 1000){
        printf("Invalid maximum iteration!");
        free_vectors(head_vec);
        free(head_cord);
        return 1;
        }
    
    n_array = create_n_Array(head_vec, num_of_lines, d);
    if (have_error == 1){
        printf("An Error Has Occurred");
        return 1;
    }

    new_centroid_array = calloc(k, sizeof(double *));
    if (new_centroid_array == NULL) {
        printf("An Error Has Occurred");
        return 1;
    }
    old_centroid_array = calloc(k, sizeof(double *));
    if (old_centroid_array == NULL) {
        printf("An Error Has Occurred");
        return 1;
    }
    for (i = 0; i < k; i++)
    {
        old_centroid_array[i] = calloc(d, sizeof(double));
        if (old_centroid_array[i] == NULL) {
            printf("An Error Has Occurred");
            return 1;
        }
        new_centroid_array[i] = calloc(d, sizeof(double));
        if (new_centroid_array[i] == NULL) {
            printf("An Error Has Occurred");
            return 1;
        }
        for (j = 0; j < d; j++){
            old_centroid_array[i][j] = n_array[i][j];
            new_centroid_array[i][j] = 0.0;
        }
    }

    total_cluster = calloc(k, sizeof(double *));
    if (total_cluster == NULL) {
            printf("An Error Has Occurred");
            return 1;
    }
    count_cluster = calloc(k, sizeof(int));
    if (count_cluster == NULL) {
            printf("An Error Has Occurred");
            return 1;
    }
    for (i = 0; i < k; i++)
    {
        total_cluster[i] = calloc(d, sizeof(double));
    if (total_cluster[i] == NULL) {
            printf("An Error Has Occurred");
            return 1;
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
            if (delta >= EPSILON){
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
        for (j = 0; j < d; j++)
        {
            if (j < d - 1)
            {
                printf("%.4f,", new_centroid_array[i][j]);
            }
            else
            {
                printf("%.4f\n", new_centroid_array[i][j]);
            }   
        }
    }
    printf("\n");

    for (i = 0; i < k; i++)
    {
        free(old_centroid_array[i]);
        free(new_centroid_array[i]);
        free(total_cluster[i]);
    }
    for (i = 0; i < num_of_lines; i++)
    {
        free(n_array[i]);
    }

    free_vectors(head_vec);
    free(head_cord);
    
    free(old_centroid_array);
    free(new_centroid_array);
    free(total_cluster);
    free(count_cluster);
    free(n_array);

    return 0;
        
}
