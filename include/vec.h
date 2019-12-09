//
// Created by root on 25/11/19.
//

#ifndef GFX_A_IV_VEC_H
#define GFX_A_IV_VEC_H

#include <cmath>


struct vec3 {
	float x, y, z;

	vec3() = default;

	vec3(float a, float b, float c) : x(a), y(b), z(c) {};

	float& operator[](size_t index);;

	const float& operator[](size_t index) const;;

	vec3 operator-(const vec3& other) const;

	vec3 operator-() const;

	vec3 operator+(const vec3& other) const;

	void operator+=(const vec3& other);

	void operator-=(const vec3& other);

	bool operator==(const vec3& other) const;

	bool operator < (const vec3& other) const;

	[[nodiscard]] float dot(const vec3& other) const;

	[[nodiscard]] vec3 cross(const vec3& other) const;

	[[nodiscard]] vec3 norm() const;

	float* data();
};

enum Channels
{
	x,
	y,
	z,
	w
};

struct vec4 {
	float x, y, z, w;

	
	
	template<int U , int V, int W>
	vec3 swizzle() const
	{
		const auto a = this->operator[]((U));
		const auto b = this->operator[]((V));
		const auto c = this->operator[]((W));

		return { a,b,c };
	}

	float& operator[](size_t index);;

	const float& operator[](size_t index) const;;

	vec4() = default;

	vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {};

	vec4(vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {};

	vec4 operator-(const vec4& other) const;

	vec4 operator-() const;

	vec4 operator+(const vec4& other) const;

	void operator+=(const vec4& other);

	void operator-=(const vec4& other);

	bool operator==(const vec4& other) const;

	[[nodiscard]] float dot(const vec4& other) const;

	[[nodiscard]] vec4 norm() const;

	float* data();
};

template<typename T>
vec3 operator*(const vec3 &vector, const T value)
{
	return {
		vector.x * static_cast<float>(value),
		vector.y * static_cast<float>(value),
		vector.z * static_cast<float>(value),
	};
}
template<typename T>
vec3 operator*(const T value, const vec3 &vector)
{
	return {
		vector.x * static_cast<float>(value),
		vector.y * static_cast<float>(value),
		vector.z * static_cast<float>(value),
	};
}
template<typename T>
vec4 operator*(const T value, const vec4 &vector)
{
	return {
		vector.x * static_cast<float>(value),
		vector.y * static_cast<float>(value),
		vector.z * static_cast<float>(value),
		vector.w * static_cast<float>(value)
	};
}

template<typename T>
vec4 operator*(const vec4& vector, const T value)
{
	return {
		vector.x * static_cast<float>(value),
		vector.y * static_cast<float>(value),
		vector.z * static_cast<float>(value),
		vector.w * static_cast<float>(value)
	};
}
template<typename T>
vec3 operator/(const vec3& vector, const T value)
{
	return {
		vector.x / static_cast<float>(value),
		vector.y / static_cast<float>(value),
		vector.z / static_cast<float>(value),
	};
}

template<typename T>
vec3 operator/(const T value, const vec3& vector)
{
	return {
		vector.x / static_cast<float>(value),
		vector.y / static_cast<float>(value),
		vector.z / static_cast<float>(value),
	};
}

template<typename T>
vec4 operator/(const T value, const vec4 &vector)
{
	return {
		vector.x / static_cast<float>(value),
		vector.y / static_cast<float>(value),
		vector.z / static_cast<float>(value),
		vector.w / static_cast<float>(value)
	};
}

template<typename T>
vec4 operator/(const vec4& vector, const T value)
{
	return {
		vector.x / static_cast<float>(value),
		vector.y / static_cast<float>(value),
		vector.z / static_cast<float>(value),
		vector.w / static_cast<float>(value)
	};
}

#endif //GFX_A_IV_VEC_H
