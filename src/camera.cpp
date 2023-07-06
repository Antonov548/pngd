#include "camera.h"

namespace pngd
{

camera make_camera(const glm::vec3& position, const glm::vec3& direction,
                   const glm::vec3& up, float fov, float aspect_ratio) noexcept
{
  camera c{};

  c.position = position;
  c.direction = direction;
  c.up = up;

  const auto theta{glm::radians(fov)};
  const auto h{glm::tan(theta / 2)};
  const auto height{2.f * h};
  const auto width{aspect_ratio * height};

  c.w = glm::normalize(position - direction);
  c.u = glm::normalize(glm::cross(up, c.w));
  c.v = glm::cross(c.w, c.u);

  c.horizontal = width * c.u;
  c.vertical = height * c.v;
  c.lower_left_corner = position - c.horizontal / 2.f - c.vertical / 2.f - c.w;

  return c;
}

glm::vec3 project(const ray& r, float t) noexcept
{
  return r.origin + r.direction * t;
}

}  // namespace pngd
