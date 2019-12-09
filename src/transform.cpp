//
// Created by root on 2/12/19.
//

#include "transform.h"
#include <cmath>

mat4x4 Transform::translate(const float x, const float y, const float z) {
    return {1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1};
}

mat4x4 Transform::scale(const float uniform_scale) {
    return {uniform_scale, 0, 0, 0,
            0, uniform_scale, 0, 0,
            0, 0, uniform_scale, 0,
            0, 0, 0, 1};
}

mat4x4 Transform::rotate_about_x(const radians angle) {
    return {1, 0, 0, 0,
            0, cosf(angle), sinf(angle), 0,
            0, -sinf(angle), cosf(angle), 0,
            0, 0, 0, 1};
}

mat4x4 Transform::rotate_about_y(const radians angle) {
    return {cosf(angle), 0, -sinf(angle), 0,
            0, 1, 0, 0,
            sinf(angle), 0, cosf(angle), 0,
            0, 0, 0, 1};
}

mat4x4 Transform::rotate_about_z(const radians angle) {
    return {cosf(angle), sinf(angle), 0, 0,
            -sinf(angle), cosf(angle), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
}