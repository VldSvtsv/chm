#include <stdio.h>
#include <math.h>

double sh(double x){
    double e = exp(x);
    return ((e - 1/e)/2);
}

double func1(double t, double x){
    return sh(2 * t) * (x * x + 1);
}

double func2(double t, double x){
    return -(5 * t * cos(t))/pow((1 + 2 * pow(t, 2)), 3);
}

double func3(double t, double x){
    return 2*t;
}

double integral_simpson(double a, double b, int n, double (*f)(double, double)){
    double d = (b - a)/n;
    double s1 = 0, s2 = 0;
    for(int i = 1; i<n; i+=2){
        s1 += f(a + i*d, b);
        s2 += f(a + (i+1)*d, b);
    }
    return (d/3)*(f(a,b) + f(b,b)+ 4*s1 + 2*s2);
}

double eps_integral(double a, double b, double eps, double (*f)(double, double)){
    double i, i1;
    int n = 2;
    i1 = integral_simpson(a, b, n, f);
    i = 0;
    while(fabs(i1 - i) > eps){
        i = i1;
        n *= 2;
        i1 = integral_simpson(a, b, n, f);
    }
    return i1;
}


int main(){
    double eps;
    scanf("%lf",&eps);
    double i, j;
    for (j = -2; j <= 2; j += 0.5){
        i = eps_integral(0, j, eps, func1);
        printf("%lf\n", i);
    }
    printf("_________________________\n");
    for (j = -2; j <= 2; j += 0.5){
        i = eps_integral(0, j, eps, func2);
        printf("%lf\n", i);
    }
    return 0;
}