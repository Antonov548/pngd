#include "mesh.h"

namespace pngd
{

mesh::mesh(triangle_surface s, std::unique_ptr<material> m) noexcept
    : surface(std::move(s))
    , tree(surface)
    , mat(std::move(m))
{}

bool mesh::hit(const ray& r, float t_min, float t_max, intersection& rec)
{
  rec.mat = mat.get();
  return tree.hit(r, t_min, t_max, rec);
}

}  // namespace pngd
