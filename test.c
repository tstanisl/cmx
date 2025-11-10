#include "cmx.h"
#include <stdio.h>
#include <stdlib.h>

#define INF INFINITY

#define LOG(...) fprintf(stderr, __VA_ARGS__)

#define CHECK(cnd, ...) \
    if (cnd); else (LOG(__VA_ARGS__), exit(-1))

typedef struct {
    float x;
    float y;
    uint8_t r;
} tc_s;

#define TEST(type, testcases) do {                                                \
    LOG("Testing " #type "\n");                                                   \
    for (size_t i = 0; i < sizeof(testcases) / sizeof(tc_s); ++i) {               \
        tc_s t = (testcases)[i];                                                  \
        type m = type ## _encode(t.x);                                            \
        CHECK(m.r == t.r, "\tError: encode: %g -> %x (got %x)\n", t.x, t.r, m.r); \
        float z = type ## _decode(m);                                             \
        CHECK(z == t.y,   "\tError: decode: %g -> %g (got %g)\n", t.x, t.y, z);   \
    }                                                                             \
} while (0)

static tc_s e2m1_tests[] =  {
    // test cases
    {  0.0f,    0.0f, 0b0'0000'000 },
    { -1.0f,   -1.0f, 0b1'0111'000 },
    {  1.0f,    1.0f, 0b0'0111'000 },
    {   NAN,     NAN, 0b1'1111'111 },
};

static tc_s e4m3_tests[] =  {
    // test cases
    {  0.0f,    0.0f, 0b0'0000'000 },
    { -1.0f,   -1.0f, 0b1'0111'000 },
    {  1.0f,    1.0f, 0b0'0111'000 },
    {   NAN,     NAN, 0b1'1111'111 },
};

int main() {
    TEST(cmxe2m1, e2m1_tests);
    TEST(cmxe4m3, e4m3_tests);
}
