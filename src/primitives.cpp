#include "primitives.h"

#include <glm/gtx/norm.hpp>

#include "camera.h"

namespace pngd
{

sphere::sphere(const glm::vec3& c, float r) noexcept : center(c), radius(r) {}

bool sphere::hit(const ray& r, float t_min, float t_max, intersection& rec)
{
  glm::vec3 oc{r.origin - center};
  const auto a{glm::length2(r.direction)};
  const auto half_b{glm::dot(oc, r.direction)};
  const auto c{glm::length2(oc) - radius * radius};

  const auto discriminant{half_b * half_b - a * c};
  if (discriminant < 0)
    return false;
  const auto sqrtd{std::sqrt(discriminant)};

  auto root{(-half_b - sqrtd) / a};
  if (root < t_min || t_max < root)
  {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.point = project(r, rec.t);

  return true;
}

}  // namespace pngd
