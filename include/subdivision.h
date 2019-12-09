//
// Created by root on 26/11/19.
//

#ifndef GFX_A_IV_SUBDIVISION_H
#define GFX_A_IV_SUBDIVISION_H

#include <vector>
#include "vec.h"

struct curve;

struct vertex
{
	vec3 position;
	vec3 normal;
	vec3 uv;

	vertex() = default;

	vertex(vec3 p, vec3 n, vec3 u) : position(p), normal(n), uv(u) {};

	bool operator ==(const vertex& other) const
	{
		return position == other.position
			&& normal == other.normal
			&& uv == other.uv;
	}

	bool operator<(const vertex& other) const
	{
		return position.x < other.position.x
			&& position.y < other.position.y
			&& position.z < other.position.z;
	}
};

inline vec3 midpoint(vec3 a, vec3 b)
{
    return (a + b) / 2.0f;
}


template <typename T>
struct tri
{
	T a, b, c;

	tri(T v1, T v2, T v3)
	{
		a = v1;
		b = v2;
		c = v3;
	}

	bool operator<(tri other)
	{
		auto& [d, e, f] = other;
		return a < d && b < e && c < f;
	}
};

inline vec3 normal_of(const vec3 &a, const vec3 &b, const vec3 &c)
{
	const auto ab = b - a;
	const auto ac = c - a;

	return ab.cross(ac).norm();
}

std::vector<vec3> subdivide(std::vector<vec3>& points, int n = 0);

std::vector<tri<vertex>> enmesh(curve curve);

#endif //GFX_A_IV_SUBDIVISION_H
