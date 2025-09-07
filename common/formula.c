#include <stddef.h>
#include <math.h>

// return mean
double find_mean(double* data, size_t length)
{
    double mean = 0.0;
    size_t i;
    for (i=0;i<length;i++) {
        mean += data[i];
    }
    mean = mean / (double)length;
    return mean;
}

// return population standard deviation
double find_stddev(double* data, size_t length, double mean)
{
    double std_dev = 0.0;
    size_t i;
    for (i=0;i<length;i++) {
        std_dev += (data[i]-mean)*(data[i]-mean);
    }
    std_dev = std_dev / (double)length;
    std_dev = sqrt(std_dev);
    return std_dev;
}
