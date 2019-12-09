//
// Created by root on 2/12/19.
//

#ifndef GFX_A_IV_SPHERE_H
#define GFX_A_IV_SPHERE_H

#include "vec.h"

struct sphere
{
	vec3 origin;
	float radius;

	sphere(vec3 o, float r) : origin(o), radius(r) {}
};

#include <utility>
#include <algorithm>
#include "vec.h"

#endif //GFX_A_IV_SPHERE_H
