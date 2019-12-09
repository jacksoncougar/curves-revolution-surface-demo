//
// Created by root on 26/11/19.
//

#include "subdivision.h"
#include "transform.h"
#include "curve.h"

std::vector<vec3> subdivide(std::vector<vec3>& points, int n)
{
	if (n < 1) return points;
	
	std::vector<vec3> result;
	result.reserve((size_t)(points.size() * 1.5));
	result.push_back(points[0]);
	result.push_back(0.5 * (points[0] + points[1]));
	for (unsigned int i = 0, j = 1, k = 2; k < points.size(); i++, j++, ++k)
	{
		auto pi = points[i];
		auto pj = points[j];
		auto pk = points[k];

		auto pijk = 1.0 / 8.0 *pi + 3.0 / 4.0 * pj + 1.0 / 8.0 * pk;
		auto pjk = 0.5 * pj + 0.5 * pk;

		result.push_back(pijk);
		result.push_back(pjk);
	}

	result.push_back(points[points.size() - 1]);
	return subdivide(result, n - 1);
}

std::vector<tri<vertex>> enmesh(curve curve)
{
	auto sphere_map = [](vec3 position, vec3 center = vec3{ 0,0,0 }) {
		auto n = (position - vec3{ 0,position.y,0 }).norm();

		float u = std::abs(std::atan2(n.z, n.x)) / (2.0f * M_PI) + 0.5;
		float v = position.y;

		return vec3{ u,v,1 };
	};

	const int amount = 30;
	std::vector<tri<vertex>> result;
	auto angle = 2.0 * M_PI / (amount);;
	for (size_t n = 0; n < amount; n++)
	{
		auto T1 = Transform::rotate_about_y(angle * n);
		auto T2 = Transform::rotate_about_y(angle * ((n + 1))); 
		for (size_t i = 0; i < curve.points.size() - 1; i++)
		{
			auto v0 = (T2 * vec4{ curve.points[i], 1 }).swizzle<0, 1, 2>();
			auto v1 = (T1 * vec4{ curve.points[i], 1 }).swizzle<0, 1, 2>();
			auto v2 = (T1 * vec4{ curve.points[i + 1], 1 }).swizzle<0, 1, 2>();
			auto v3 = (T2 * vec4{ curve.points[i + 1], 1 }).swizzle<0, 1, 2>();

			result.emplace_back(vertex{ v0, vec3{ 0,0,0 }, sphere_map(v0) }, vertex{ v2, vec3{ 0,0,0 }, sphere_map(v2) }, vertex{ v1, vec3{ 0,0,0 }, sphere_map(v1) });
			result.emplace_back(vertex{ v2, vec3{ 0,0,0 }, sphere_map(v2) }, vertex{ v0, vec3{ 0,0,0 }, sphere_map(v0) }, vertex{ v3, vec3{ 0,0,0 }, sphere_map(v3) });
		}
	}
	return result;
}