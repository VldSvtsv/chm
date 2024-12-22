#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#define ARR_SIZE 10
#define M_PI 3.14159265358979323846

struct array{
    double *x_array;
    double *y_array;
    int len;
    int _size; 
};

struct array *creat_array(){
    struct array *arr = malloc(sizeof(struct array));
    if(arr == NULL) exit(1);
    arr->len = 0;
    arr->_size = ARR_SIZE;
    arr->x_array =  calloc(arr->_size, sizeof(double));
    if(arr->x_array == NULL) exit(1);
    arr->y_array = calloc(arr->_size, sizeof(double));
    if(arr->y_array == NULL) exit(1);
    return arr;
}

void free_array(struct array *arr){
    free(arr->x_array);
    free(arr->y_array);
    free(arr);
}

void add_value(struct array *arr, double x, double y){
    if(arr->len == arr->_size){
        arr->_size *= 2;
        double *temp = realloc(arr->x_array, arr->_size * sizeof(double));
        if(temp == NULL){
            free_array(arr);
            exit(1);
        }
        arr->x_array = temp;

        temp = realloc(arr->y_array, arr->_size * sizeof(double));
        if(temp == NULL){
            free_array(arr);
            exit(1);
        }
        arr->y_array = temp;
    }
    arr->x_array[arr->len] = x;
    arr->y_array[arr->len++] = y;
}

double f1(double t, double x){
    return sinh(2 * t) * (x * x + 1);
}

double f2(double t, double x){
    return -(5 * t * cos(t)) / pow((1 + 2 * pow(t, 2)), 3);
}

double f3(double t, double x){
    return 2*t;
}

double integral_simpson(double a, double b, int n, double (*f)(double, double)){
    double d = (b - a) / n;
    double s1 = 0, s2 = 0;
    for(int i = 1; i < n; i+=2){
        s1 += f(a + i * d, b);
        s2 += f(a + (i + 1) * d, b);
    }
    return (d/3) * (f(a,b) + f(b,b) + 4*s1 + 2*s2);
}


double lagrange(struct array *arr, double x){
    double P = 0;
    double l;
    for(int i = 0; i < arr->len; i++){
        l = arr->y_array[i];
        for(int j = 0; j < arr->len; j++){
            if(i != j) l *= (x - arr->x_array[j]) / (arr->x_array[i] - arr->x_array[j]);
        }
        P += l;
    }
    return P;
}

void output(double a, double b, double s1, double s2, int n, double (*f)(double, double)){
    struct array *arr1 = creat_array();
    for (double j = a; j <= b; j += s1){
        add_value(arr1, j, integral_simpson(0, j, n, f));
        printf("x: %*lf y: %*lf\n", 10, arr1->x_array[arr1->len-1] ,10, arr1->y_array[arr1->len-1]);
    }
    printf("\n");
    
    int nodes = ((b - a)/ s1) + 1;
    struct array *arr_chebyshov = creat_array();
    printf("Function + Chebyshov nodes\n");
    for (double j = 1; j <= nodes; j++){
        double x = (a + b + (b - a)*cos(M_PI*(2*j-1)/(2*nodes)))*0.5;
        add_value(arr_chebyshov, x, integral_simpson(0, x, n, f));
        printf("x: %*lf y: %*lf\n", 10, arr_chebyshov->x_array[arr_chebyshov->len-1] ,10, arr_chebyshov->y_array[arr_chebyshov->len-1]);
    }

    printf("\n\nx : ");
    for(double j = a; j <= b; j += s2) printf("%.1lf ", j);
    printf("\n\nf(x) : ");
    struct array *arr2 = creat_array();
    for(double j = a; j <= b; j += s2) {
        add_value(arr2, j, integral_simpson(0, j, n, f));
        printf("%lf ", arr2->y_array[arr2->len-1]);
    }
    printf("\n\nP(x) : ");
    struct array *arr3 = creat_array();
    for(double j = a; j <= b; j += s2){
        add_value(arr3, j, lagrange(arr1, j));
        printf("%lf ", arr3->y_array[arr3->len-1]);
    }
    printf("\n\nP(x) + Chebyshov nodes : ");
    struct array *arr4 = creat_array();
    for(double j = a; j <= b; j += s2){
        add_value(arr4, j, lagrange(arr_chebyshov, j));
        printf("%lf ", arr4->y_array[arr4->len-1]);
    }
    int m = ((b - a)/s2);
    printf("\n\nabs : ");
    for(int j = 0; j < m; j++) printf("%lf ", fabs(arr2->y_array[j] - arr3->y_array[j]));
    printf("\n\nrel : ");
    for(int j = 0; j < m; j++) printf("%lf ", fabs(arr3->y_array[j] / arr2->y_array[j]));

    printf("\n\nabs chebyshov : ");
    for(int j = 0; j < m; j++) printf("%lf ", fabs(arr2->y_array[j] - arr4->y_array[j]));
    printf("\n\nrel chebyshov: ");
    for(int j = 0; j < m; j++) printf("%lf ", fabs(arr4->y_array[j] / arr2->y_array[j]));


    printf("\n");
    
    free_array(arr2);
    free_array(arr3);
    free_array(arr1);
    free_array(arr4);
    free_array(arr_chebyshov);
}



int main(){
    int n = 1000;//кол-во разбиений интеграла
    double s1 = 0.5, s2  = 0.1;//шаг
    double a = -2, b = 2;//граници

    printf("\n________Function 1_________\n");
    output(a, b, s1, s2, n, f1);
    printf("\n________Function 2_________\n");
    output(a, b, s1, s2, n, f2);
    
    return 0;
}