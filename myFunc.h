#ifndef MYFUNC_H
#define MYFUNC_H
#include<cmath>
#include <cstdlib>
#include <limits>
#include <memory>

#include "Ray.h"
#include "myVector.h"

const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#define rndm 0x100000000LL
#define rndc 0xB16
#define rnda 0x5DEECE66DLL
static unsigned long long seed = 1;
//drand48是个生成[0, 1)之间的均匀随机数函数。
inline double drand48(void)
{
    seed = (rnda * seed + rndc) & 0xFFFFFFFFFFFFLL;
    unsigned int x = seed >> 16;
    return  ((double)x / (double)rndm);
}
#endif