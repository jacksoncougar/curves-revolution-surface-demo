//
// Created by root on 25/11/19.
//

#include "vec.h"

float *vec3::data() {
    return &x;
}

vec3 vec3::norm() const {
    auto length = std::sqrt(x * x + y * y + z * z);
    return {x / length,y / length,z / length};
}

vec3 vec3::cross(const vec3 &other) const {
    auto &[u1, u2, u3] = *this;
    auto &[v1, v2, v3] = other;
    return {
            u2 * v3 - u3 * v2,
            u3 * v1 - u1 * v3,
            u1 * v2 - u2 * v1
    };
}

float vec3::dot(const vec3 &other) const {
    return x * other.x + y * other.y + z * other.z;
}

vec3 vec3::operator+(const vec3 &other) const {
    return {x + other.x,
            y + other.y,
            z + other.z};
}

vec3 vec3::operator-(const vec3 &other) const {
    return {x - other.x,
            y - other.y,
            z - other.z};
}

vec3 vec3::operator-() const
{
	return  { -x, -y, -z };
}

const float &vec3::operator[](const size_t index) const {
    return *(&x + index);
}

float &vec3::operator[](const size_t index) {
    return *(&x + index);
}

void vec3::operator+=(const vec3 &other)  {
    x += other.x;
    y += other.y;
    z += other.z;
}

void vec3::operator-=(const vec3 &other)  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

bool vec3::operator==(const vec3& other) const
{
	return std::abs(x - other.x) < 0.00001
		&& std::abs(y - other.y) < 0.00001
		&& std::abs(z - other.z) < 0.00001;
}

bool vec3::operator<(const vec3& other) const
{
	auto& [d, e, f] = other;
	return x < d && y < e && z < f;
}


// VEC 4

float *vec4::data() {
    return &x;
}

vec4 vec4::norm() const {
    auto length = std::sqrt(x * x + y * y + z * z + w * w);
    return {x / length,y / length,z / length, w/length};
}


float vec4::dot(const vec4 &other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

vec4 vec4::operator+(const vec4 &other) const {
    return {x + other.x,
            y + other.y,
            z + other.z,
            w + other.w
    };
}

vec4 vec4::operator-(const vec4 &other) const {
    return {x - other.x,
            y - other.y,
            z - other.z,
    w - other.w};
}

vec4 vec4::operator-() const
{
	return { -x, -y, -z, -w};
}

const float &vec4::operator[](const size_t index) const {
    return *(&x + index);
}

float &vec4::operator[](const size_t index) {
    return *(&x + index);
}

void vec4::operator+=(const vec4 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
}

void vec4::operator-=(const vec4 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
}

bool vec4::operator==(const vec4& other) const
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}
