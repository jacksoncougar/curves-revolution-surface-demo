//
// Created by root on 13/10/19.
//

#include "camera.h"

struct mat4x4 Camera::create_perspective_matrix(Camera::Params parameters)
{
  const auto aspect_ratio = parameters.image_width / parameters.image_height;
  auto t = tanf(parameters.fov * 0.5f) * parameters.near_clipping_distance;
  auto b = -t;
  auto l = b;
  auto r = -l;
  const auto n = parameters.near_clipping_distance;
  const auto f = parameters.far_clipping_distance;

  if (aspect_ratio < 1)
  {
    b /= aspect_ratio;
    t /= aspect_ratio;
  }
  else
  {
    l *= aspect_ratio;
    r *= aspect_ratio;
  }

  // clang-format off
    return {
	    2 * n / (r -  l), 0, (r + l) / (r - l), 0,
	    0, 2 * n / (t - b), (t + b) / (t - b), 0,
	    0, 0, -(f +   n) / (f - n), -2 * f * n / (f - n),
	    0, 0, -1, 0
    };
  // clang-format on
}

struct mat4x4 Camera::create_orthographic_matrix(Camera::Params parameters)
{
  auto ratio = parameters.image_height / parameters.image_width;

  auto l = -1.0f;
  auto t = ratio;

  const auto tall = parameters.image_width < parameters.image_height;
  if (!tall)
  {
    ratio = parameters.image_width / parameters.image_height;
    l = -ratio;
    t = 1;
  }

  const auto b = -t;
  const auto r = -l;

  const auto n = parameters.near_clipping_distance;
  const auto f = parameters.far_clipping_distance;

  // clang-format off
	return { 2 / (r - l), 0, 0, 0,
			0, 2 / (t - b), 0, 0,
			0, 0, -2 / (f - n), 0,
			-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1
	};
  // clang-format on
}
