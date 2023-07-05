#pragma once

#include "types.h"

namespace pngd
{

struct sphere : public hittable
{
  sphere(const glm::vec3& c, float r) noexcept;

  glm::vec3 center;
  float radius;

  bool hit(const ray& r, float t_min, float t_max, intersection& rec) final;
};

}  // namespace pngd
