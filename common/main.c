#include "formula.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 5;
    double* test = malloc(n*sizeof(double));
    for (size_t i=0;i<n;i++) {
        test[i] = i;
    }
    double m = mean(test, n);
    printf("%f\n",m);
}
