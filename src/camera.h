#pragma once

#include "types.h"

namespace pngd
{

struct camera
{
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 up;

  glm::vec3 horizontal;
  glm::vec3 vertical;
  glm::vec3 lower_left_corner;

  glm::vec3 w, u, v;
};

camera make_camera(const glm::vec3& position, const glm::vec3& direction,
                   const glm::vec3& up, float fov, float aspect_ratio) noexcept;

glm::vec3 project(const ray& r, float t) noexcept;

}  // namespace pngd
