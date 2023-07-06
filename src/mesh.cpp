#include "mesh.h"

namespace pngd
{

mesh::mesh(triangle_surface s) noexcept : surface(std::move(s)) {}

bool mesh::hit(const ray& r, float t_min, float t_max, intersection& rec)
{
  return false;
}

}  // namespace pngd
