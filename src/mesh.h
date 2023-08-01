#pragma once

#include "kdtree.h"
#include "types.h"

namespace pngd
{

struct mesh : public hittable
{
  mesh(triangle_surface s, std::unique_ptr<material> m) noexcept;

  triangle_surface surface;
  kd_tree tree;
  std::unique_ptr<material> mat;

  bool hit(const ray& r, float t_min, float t_max, intersection& rec) final;
};

}  // namespace pngd
