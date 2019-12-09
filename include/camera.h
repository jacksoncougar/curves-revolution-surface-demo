//
// Created by root on 13/10/19.
//

#ifndef GRAPHICS_ASSIGNMENT_II_CAMERA_H
#define GRAPHICS_ASSIGNMENT_II_CAMERA_H

#include <functional>
#include "vec.h"
#include "transform.h"
#include <cmath>
#include <algorithm>
#include "units.h"

class Camera {

    float yaw = 0.0f;
    float pitch = 0.0f;
    float zoom = 0.5f;
    vec4 position{0, 0, 0, 1};

public:

    typedef float delta;

    struct mat4x4 transform(vec4 displacement) {
        position += displacement;
        return Transform::scale(this->zoom)
               * Transform::rotate_about_y(this->yaw)
               * Transform::rotate_about_x(this->pitch)
               * Transform::translate(position.x, position.y, position.z);
    }

    struct mat4x4 transform(delta yaw, delta pitch, delta zoom) {

        this->zoom = std::clamp(this->zoom + zoom, 0.02f, 100.f);
        this->yaw = (float)fmod(this->yaw + yaw, 2.f * M_PI);
        this->pitch = (float)fmod(this->pitch + pitch, 2.f * M_PI);

        return Transform::scale(this->zoom)
               * Transform::rotate_about_y(this->yaw)
               * Transform::rotate_about_x(this->pitch)
               * Transform::translate(position.x, position.y, position.z);
    }

    struct Params {
        float image_width;
        float image_height;
        float fov = (float)30._deg;
        float near_clipping_distance = 0.1f;
        float far_clipping_distance = 50.0f;
    };

    struct Frustrum {
        float l = -1.0f;
        float r = 1.0;
        float t = 1.0f;
        float b = -1.0f;
        float n = -1.0f;
        float f = 1.0f;
        float a = 1.0f;
    };

    static struct mat4x4 create_projection(Frustrum parameters) {
        auto[l, r, t, b, n, f, a] = parameters;

        if (a < 1.0) {
            b /= a;
            t /= a;
        } else {
            l *= a;
            r *= a;
        }

        return {2 / (r - l), 0, 0, -(r + l) / (r - l),
                0, 2 / (t - b), 0, -(t + b) / (t - b),
                0, 0, -2 / (f - n), -(f + n) / (f - n),
                0, 0, 0, 1};
    }

    static struct mat4x4 create_perspective_matrix(Params parameters);

	static struct mat4x4 create_orthographic_matrix(Params parameters);

	static struct mat4x4 create_lookat_matrix(vec3 eye, vec3 up, vec3 target)
	{
		up = up.norm();
		auto forward = (eye - target).norm();
		auto left = up.cross(forward).norm();
		auto [x, y, z] = -eye;
		return mat4x4::from_columns(
			{ left, x },
			{ up, y },
			{ forward, z },
			{ 0,0,0,1 });
	}
};

#endif //GRAPHICS_ASSIGNMENT_II_CAMERA_H
