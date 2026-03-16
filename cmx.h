#ifndef CMX_H
#define CMX_H __FILE__

#include <stdint.h>
#include <math.h>

// r - representation as uint8, s - sign, m - mantissa, e - exponenent
typedef union { uint8_t r; struct { uint8_t m:1, e:2, s:1, _:4; }; } cmxe2m1;
typedef union { uint8_t r; struct { uint8_t m:3, e:4, s:1;      }; } cmxe4m3;

// helper for conversions from float
typedef union { float f; uint32_t r; struct { uint32_t m:23, e:8, s:1; }; } cmxf32;

#define CMXE2M1_MAX 6.0f
#define CMXE4M3_MAX 448.0f

#define CMX_MAX(T) \
_Generic((T){0}          \
  , cmxe2m1: CMXE2M1_MAX \
  , cmxe4m3: CMXE4M3_MAX \
)

static inline cmxe2m1 cmxe2m1_encode(float f) {
    if (f < -CMXE2M1_MAX) f = -CMXE2M1_MAX;
    if (f > +CMXE2M1_MAX) f = +CMXE2M1_MAX;
    cmxf32 c = { .f = f * 0x1p-126f };
    c.r += 1u << 21;
    return (cmxe2m1) { .s = c.s, .e = c.e, .m = c.m >> 22};
}

static inline float cmxe2m1_decode(cmxe2m1 m) {
    cmxf32 c = { .s = m.s, .e = m.e, .m = m.m << 22 };
    return c.f * 0x1p126f;
}

static inline cmxe4m3 cmxe4m3_encode(float f) {
    if (isnan(f)) return (cmxe4m3) { .r = 0x7f };
    if (f < -CMXE4M3_MAX) f = -CMXE4M3_MAX;
    if (f > +CMXE4M3_MAX) f = +CMXE4M3_MAX;
    cmxf32 c = { .f = f * 0x1p-120f };
    c.r += 1u << 19;
    return (cmxe4m3) { .s = c.s, .e = c.e, .m = c.m >> 20};
}

static inline float cmxe4m3_decode(cmxe4m3 m) {
    if ((~m.r & 0x7f) == 0) return NAN;
    cmxf32 c = { .s = m.s, .e = m.e, .m = m.m << 20 };
    return c.f * 0x1p120f;
}

#endif

