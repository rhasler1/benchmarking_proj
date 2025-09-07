#include <stddef.h>

#ifndef FORMULA_H
#define FORMULA_H

double find_mean(double* data, size_t length);
double find_stddev(double* data, size_t length, double mean);

#endif
