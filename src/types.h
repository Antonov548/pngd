#pragma once

#include <vector>
#include <array>
#include <cstdint>
#include <glm/glm.hpp>

namespace pngd
{

struct rgba
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

using buffer = std::vector<std::byte>;

using bitmap = std::vector<uint8_t>;

struct ray
{
  glm::vec3 origin;
  glm::vec3 direction;
};

struct intersection
{
  glm::vec3 point;
  glm::vec3 normal;

  float t;
  bool front_face;
};

using triangle = std::array<size_t, 3>;

struct surface
{
  std::vector<glm::vec3> vertices;
  std::vector<triangle> triangles;
};

struct hittable
{
  virtual ~hittable() = default;
  virtual bool hit(const ray& r, float t_min, float t_max, intersection& rec) = 0;
};

} // namespace pngd
