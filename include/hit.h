//
// Created by root on 2/12/19.
//

#ifndef GFX_A_IV_HIT_H
#define GFX_A_IV_HIT_H

#include <algorithm>
#include "ray.h"
#include "sphere.h"

struct hit
{
	bool did_intersect = false;
	float t = std::numeric_limits<float>::infinity();

	explicit operator bool() const {
		return 	did_intersect;
	}
};

template<typename T>
struct traceable
{
	T surface;
	template<typename... Args>
	explicit traceable(Args&&... args) : surface(std::forward<Args>(args)...) {};

	hit test(ray ray) {
		return intersect(ray, surface);
	}

	bool operator==(const traceable<T>& other) const
	{
		auto a0 = &other.surface;
		auto a1 = &surface;
		return  a0 == a1;
	}
};

hit intersect(ray ray, sphere sphere);

#include "ray.h"
#include "sphere.h"

#endif //GFX_A_IV_HIT_H
