//
// Created by root on 2/12/19.
//

#include "matrix.h"

mat4x4::mat4x4(float m00, float m01, float m02, float m03,
               float m10, float m11, float m12, float m13,
               float m20, float m21, float m22, float m23,
               float m30, float m31, float m32, float m33)
        : m00(m00), m01(m01), m02(m02), m03(m03),
          m10(m10), m11(m11), m12(m12), m13(m13),
          m20(m20), m21(m21), m22(m22), m23(m23),
          m30(m30), m31(m31), m32(m32), m33(m33) {}

mat4x4::mat4x4() : mat4x4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1) {}

mat4x4 mat4x4::operator*(const mat4x4 &o) {
    return {
            m00 * o.m00 + m01 * o.m10 + m02 * o.m20 + m03 * o.m30,
            m00 * o.m01 + m01 * o.m11 + m02 * o.m21 + m03 * o.m31,
            m00 * o.m02 + m01 * o.m12 + m02 * o.m22 + m03 * o.m32,
            m00 * o.m03 + m01 * o.m13 + m02 * o.m23 + m03 * o.m33,

            m10 * o.m00 + m11 * o.m10 + m12 * o.m20 + m13 * o.m30,
            m10 * o.m01 + m11 * o.m11 + m12 * o.m21 + m13 * o.m31,
            m10 * o.m02 + m11 * o.m12 + m12 * o.m22 + m13 * o.m32,
            m10 * o.m03 + m11 * o.m13 + m12 * o.m23 + m13 * o.m33,

            m20 * o.m00 + m21 * o.m10 + m22 * o.m20 + m23 * o.m30,
            m20 * o.m01 + m21 * o.m11 + m22 * o.m21 + m23 * o.m31,
            m20 * o.m02 + m21 * o.m12 + m22 * o.m22 + m23 * o.m32,
            m20 * o.m03 + m21 * o.m13 + m22 * o.m23 + m23 * o.m33,

            m30 * o.m00 + m31 * o.m10 + m32 * o.m20 + m33 * o.m30,
            m30 * o.m01 + m31 * o.m11 + m32 * o.m21 + m33 * o.m31,
            m30 * o.m02 + m31 * o.m12 + m32 * o.m22 + m33 * o.m32,
            m30 * o.m03 + m31 * o.m13 + m32 * o.m23 + m33 * o.m33};
}

vec4 mat4x4::operator*(const vec4& vector) const
{
	auto& [x, y, z, w] = vector;
	return {
			m00* x + m01 * y + m02 * z + m03 * w,
			m10* x + m11 * y + m12 * z + m13 * w,
			m20* x + m21 * y + m22 * z + m23 * w,
			m30* x + m31 * y + m32 * z + m33 * w
	};
}

mat4x4 mat4x4::from_columns(vec4 c0, vec4 c1, vec4 c2, vec4 c3)
{
	return mat4x4(c0[0], c1[0], c2[0], c3[0],
		c0[1], c1[1], c2[1], c3[1],
		c0[2], c1[2], c2[2], c3[2],
		c0[3], c1[3], c2[3], c3[3]);
}

mat4x4 mat4x4::from_rows(vec4 c0, vec4 c1, vec4 c2, vec4 c3)
{
	return mat4x4(c0[0], c0[1], c0[2], c0[3],
		c1[0], c1[1], c1[2], c1[3],
		c2[0], c2[1], c2[2], c2[3],
		c3[0], c3[1], c3[2], c3[3]);
}

float *mat4x4::data() { return &m00; }
