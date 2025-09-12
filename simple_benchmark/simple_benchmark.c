#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    long n = atol(argv[1]);

    for (long k = 0; k < n; k++) {
        asm("nop");
    }

    return 0;
}
