//
// Created by root on 25/11/19.
//

#ifndef GFX_A_IV_RAY_H
#define GFX_A_IV_RAY_H


#include "vec.h"

#include <utility>
#include <algorithm>

struct ray {
    vec3 origin;
    vec3 direction;

    /**
     * returns the point on the ray
     * @param t the parameterized distance along the ray
     */
    [[nodiscard]] inline vec3 point_at(const float t) const
    {
        return origin + t * direction;
    }
};

#endif //GFX_A_IV_RAY_H
