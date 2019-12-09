//
// Created by root on 2/12/19.
//

#ifndef GFX_A_IV_MATRIX_H
#define GFX_A_IV_MATRIX_H

#include "vec.h"

struct mat4x4 {
    float m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33;

    float *data();

    mat4x4();

    mat4x4(float m00, float m01, float m02, float m03,
           float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33);

    [[nodiscard]] vec4 rows(const int index) const
	{
		const auto offset = index * 4;
		const auto start = &m00;

		return {
			*(start + offset + 0),
			*(start + offset + 1),
			*(start + offset + 2),
			*(start + offset + 3),
		};
	}

	[[nodiscard]] vec4 columns(const int index) const
	{
		const auto stride = 4;
		const auto start = (&m00) + index;

		return {
			*(start + stride * 0),
			*(start + stride * 1),
			*(start + stride * 2),
			*(start + stride * 3),
		};
	}
	
    mat4x4 operator*(const mat4x4 &);

    vec4 operator*(const vec4 &vector) const;

	static mat4x4 from_columns(vec4 c0, vec4 c1, vec4 c2, vec4 c3 = vec4{ 0,0,0,1 });
	static mat4x4 from_rows(vec4 c0, vec4 c1, vec4 c2, vec4 c3);
};


#endif //GFX_A_IV_MATRIX_H
