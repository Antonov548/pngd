#include "camera.h"

namespace pngd
{

glm::vec3 project(const ray& r, float t) noexcept
{
  return r.origin + r.direction * t;
}

}  // namespace pngd
