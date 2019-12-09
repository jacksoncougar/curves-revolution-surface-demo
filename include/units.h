//
// Created by root on 2/12/19.
//

#ifndef GFX_A_IV_UNITS_H
#define GFX_A_IV_UNITS_H

#include <cmath>

constexpr long double operator "" _deg(long double value) {
    return value * M_PI / 180.;
}

#endif //GFX_A_IV_UNITS_H
