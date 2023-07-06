#pragma once

#include "types.h"

namespace pngd
{

struct mesh : public hittable
{
  explicit mesh(triangle_surface s) noexcept;

  triangle_surface surface;

  bool hit(const ray& r, float t_min, float t_max, intersection& rec) final;
};

}  // namespace pngd
