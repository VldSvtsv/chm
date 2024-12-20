#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#define ARR_SIZE 10

struct array{
    double *x_array;
    double *y_array;
    int len;
    int _size; 
};

struct array *creat_array(){
    struct array *arr = (struct array *) malloc(sizeof(struct array));
    if(arr == NULL) exit(1);
    arr->len = 0;
    arr->_size = ARR_SIZE;
    arr->x_array = (double *) calloc(arr->_size, sizeof(double));
    if(arr->x_array == NULL) exit(1);
    arr->y_array = (double *) calloc(arr->_size, sizeof(double));
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
        double *temp = (double *) realloc(arr->x_array, arr->_size * sizeof(double));
        if(temp == NULL){
            free_array(arr);
            exit(1);
        }
        arr->x_array = temp;

        temp = (double *) realloc(arr->y_array, arr->_size * sizeof(double));
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


int main(){
    int n = 1000;//кол-во разбиений интеграла
    double s = 0.5;//шаг
    double a = -2, b = 2;//граници
    int nodes = ((b - a)/ s) + 1;//кол-во узлов Чебышева
    printf("Nodes = %d\n", nodes);

    struct array *arr = creat_array();
    printf("\nFunction 1\nn = %d\n", n);
    for (double j = a; j <= b; j += s){
        add_value(arr, j, integral_simpson(0, j, n, f1));
        printf("x: %*lf y: %*lf\n", 10, arr->x_array[arr->len-1] ,10, arr->y_array[arr->len-1]);
    }
    free_array(arr);

    arr = creat_array();
    printf("\nFunction 1 + Chebyshov nodes\nn = %d\n", n);
    for (double j = 1; j <= nodes; j++){
        double x = (a + b + (b - a)*cos(M_PI*(2*j-1)/(2*nodes)))*0.5;
        add_value(arr, x, integral_simpson(0, x, n, f1));
        printf("x: %*lf y: %*lf\n", 10, arr->x_array[arr->len-1] ,10, arr->y_array[arr->len-1]);
    }
    free_array(arr);
   


    arr = creat_array();
    printf("\nFunction 2\nn = %d\n", n);
    for (double j = a; j <= b; j += s){
        add_value(arr, j, integral_simpson(0, j, n, f2));
        printf("x: %*lf y: %*lf\n", 10, arr->x_array[arr->len-1] ,10, arr->y_array[arr->len-1]);
    }
    free_array(arr);

    arr = creat_array();
    printf("\nFunction 2 + Chebyshov nodes\nn = %d\n", n);
    for (double j = 1; j <= nodes; j++){
        double x = (a + b + (b - a)*cos(M_PI*(2*j-1)/(2*nodes)))*0.5;
        add_value(arr, x, integral_simpson(0, x, n, f2));
        printf("x: %*lf y: %*lf\n", 10, arr->x_array[arr->len-1] ,10, arr->y_array[arr->len-1]);
    }
    free_array(arr);
    

    return 0;
}