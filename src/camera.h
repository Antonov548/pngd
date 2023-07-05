#pragma once

#include "types.h"

namespace pngd
{

struct camera
{
  glm::vec3 position;
  glm::vec3 direction;

  glm::vec3 horizontal;
  glm::vec3 vertical;
  glm::vec3 lower_left_corner;
};

glm::vec3 project(const ray& r, float t) noexcept;

}  // namespace pngd
