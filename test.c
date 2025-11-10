#include "cmx.h"
#include <stdio.h>
#include <stdlib.h>

#define INF INFINITY

#define LOG(...) fprintf(stderr, __VA_ARGS__)

void test_e4m3() {
    LOG("Testing E4M3\n");
    float tc[][2] =  {
        // test cases
        { 0.0f,    0.0f },
        { 1.0f,    1.0f },
        {  NAN,     NAN },
    };

    for (size_t i = 0; i < sizeof tc / sizeof tc[0]; ++i) {
        float x = tc[i][0];
        float y = tc[i][1];
        auto m = cmxe4m3_encode(x);
        float z = cmxe4m3_decode(m);

        if (z != y) {
            LOG("\tError: %g -> %g (got %g)\n", x, y, z);
            exit(-1);
        }
    }
}

int main() {
    test_e4m3();
}
