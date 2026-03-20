// Test's whether the loader can actually load bionic
#include <stdio.h>
#include <math.h>

__attribute__((constructor))
void on_load() {
    double result = sqrt(16.0);
    printf("sqrt(16) = %f\n", result);
}
