#ifndef CMX_H
#define CMX_H __FILE__

#include <stdint.h>
#include <math.h>

// r - representation as uint8, s - sign, m - mantissa, e - exponenent
typedef union { uint8_t r; struct { uint8_t m:1, e:2, s:1, _:4; }; } cmxe2m1;
typedef union { uint8_t r; struct { uint8_t m:3, e:4, s:1;      }; } cmxe4m3;

// helper for conversions from float
typedef union { float f; uint32_t r; struct { uint32_t m:23, e:8, s:1; }; } cmxcvt_;

cmxe2m1 cmxe2m1_encode(float f) {
    cmxcvt_ c_ = { .f = f };
    cmxcvt_ c  = { .e = c_.e, .m = c_.m };
    c = c.f < 6 ? c : (cmxcvt_){ .f = 6 };
    c = c.f < 1 ? (cmxcvt_){ .f = 0.625f + 0.5f * c.f }
                : (cmxcvt_){ .r = c.r + (1 << 21) };
    return (cmxe2m1) { .s = c_.s, .e = c.e - 126, .m = c.m >> 22 };
}

static inline cmxe4m3 cmxe4m3_encode(float f) {
    cmxcvt_ c_ = { .f = f };
    cmxcvt_ c  = { .e = c_.e, .m = c_.m };
    c = c.f >= 448 ? (cmxcvt_){ .f = 448 } : c;
    c = c.f >= 0x1p-6 ? (cmxcvt_){ .r = c.r + (1 << 19) }
                      : (cmxcvt_){ .f = 0.5 * 0x1p-6 + 0.25f * 0x1p-9 + 0.5f * c.f };
    return (cmxe4m3) { .s = c_.s, .e = c.e - 120, .m = c.m >> 20 };
}

static inline float cmxe2m1_decode(cmxe2m1 m) {
    if (m.e == 0) return (m.s ? -0.5f : 0.5f) * m.m;
    return (cmxcvt_) { .s = m.s, .e = m.e + 126, .m = m.m << 22 }.f;
}

static inline float cmxe4m3_decode(cmxe4m3 m) {
    if (m.e == 0) return (m.s ? -0x1p-9 : 0x1p-9) * m.m;
    return (cmxcvt_) { .s = m.s, .e = m.e + 120, .m = m.m << 20 }.f;
}

#endif

