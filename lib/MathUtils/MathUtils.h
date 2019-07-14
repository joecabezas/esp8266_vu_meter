#ifndef MATH_UTILS_H
#define MATH_UTILS_H

template <class X, class M, class N, class O, class Q>
X map_generic(X x, M in_min, N in_max, O out_min, Q out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif