#include "primitives.h"

#include <glm/gtx/norm.hpp>

#include "camera.h"

namespace pngd
{

sphere::sphere(const glm::vec3& c, float r) noexcept : center(c), radius(r) {}

bool sphere::hit(const ray& r, float t_min, float t_max, intersection& rec)
{
  glm::vec3 oc = r.origin - center;
  auto a = glm::length2(r.direction);
  auto half_b = glm::dot(oc, r.direction);
  auto c = glm::length2(oc) - radius * radius;

  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root)
  {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.point = project(r, rec.t);
  glm::vec3 outward_normal = (rec.point - center) / radius;

  return true;
}

}  // namespace pngd
