//
// Created by root on 2/12/19.
//

#ifndef GFX_A_IV_TRANSFORM_H
#define GFX_A_IV_TRANSFORM_H

#include "matrix.h"


namespace Transform {

    typedef float radians;

    mat4x4 translate(float x, float y, float z);

    mat4x4 scale(float uniform_scale);

    mat4x4 rotate_about_x(radians angle);

    mat4x4 rotate_about_y(radians angle);

    mat4x4 rotate_about_z(radians angle);
};


#endif //GFX_A_IV_TRANSFORM_H
