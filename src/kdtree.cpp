#include "kdtree.h"

#include <algorithm>
#include <numeric>

#include "bbox.h"

namespace
{

bool compare(const pngd::triangle_surface& s, size_t l, size_t r,
             size_t d) noexcept
{
  const auto& tl{s.triangles[l]};
  const auto& tr{s.triangles[r]};

  const auto sl{s.vertices[tl[0]][d] + s.vertices[tl[1]][d] +
                s.vertices[tl[2]][d]};
  const auto sr{s.vertices[tr[0]][d] + s.vertices[tr[1]][d] +
                s.vertices[tr[2]][d]};

  if (sl != sr)
    return sl < sr;
  return l < r;
}

pngd::bbox make_bbox(const pngd::triangle_surface& s, size_t i) noexcept
{
  const auto& tr{s.triangles[i]};
  pngd::bbox bb{pngd::make_bbox(s.vertices[tr[0]], s.vertices[tr[1]])};

  return pngd::unite(bb, s.vertices[tr[2]]);
}

bool project(const pngd::ray& r, double& t, glm::vec3& bp, const glm::vec3& p0,
             const glm::vec3& p1, const glm::vec3& p2, float t_min,
             float t_max) noexcept
{
  const auto e1{p1 - p0};
  const auto e2{p2 - p0};
  const auto pv{glm::cross(r.direction, e2)};
  const auto det{glm::dot(e1, pv)};

  if (det == 0.f)
    return false;

  const auto invdet{1.f / det};
  const auto tv{r.origin - p0};
  bp[1] = glm::dot(tv, pv) * invdet;
  if (bp[1] < 0.f || bp[1] > 1.f)
    return false;
  const auto qv{glm::cross(tv, e1)};
  bp[2] = glm::dot(r.direction, qv) * invdet;
  if (bp[2] < 0.f || bp[1] + bp[2] > 1.f)
    return false;
  t = glm::dot(e2, qv) * invdet;
  bp[0] = 1 - bp[1] - bp[2];
  if (t < t_min)
    return false;
  if (t > t_max)
    return false;
  return true;
}

bool project(const pngd::ray& r, const pngd::bbox& bb, float& t_min,
             float& t_max)
{
  t_min = (bb.first[0] - r.origin[0]) /
          r.direction[0];  // Division by zero is allowed
  t_max = (bb.second[0] - r.origin[0]) /
          r.direction[0];  // Division by zero is allowed
  if (t_min > t_max)
    std::swap(t_min, t_max);
  auto tymin{(bb.first[1] - r.origin[1]) /
             r.direction[1]};  // Division by zero is allowed
  auto tymax{(bb.second[1] - r.origin[1]) /
             r.direction[1]};  // Division by zero is allowed
  if (tymin > tymax)
    std::swap(tymin, tymax);
  if ((t_min > tymax) || (tymin > t_max))
    return false;
  if (tymin > t_min)
    t_min = tymin;
  if (tymax < t_max)
    t_max = tymax;
  double tzmin = (bb.first[2] - r.origin[2]) /
                 r.direction[2];  // Division by zero is allowed
  double tzmax = (bb.second[2] - r.origin[2]) /
                 r.direction[2];  // Division by zero is allowed
  if (tzmin > tzmax)
    std::swap(tzmin, tzmax);
  if ((t_min > tzmax) || (tzmin > t_max))
    return false;
  if (tzmin > t_min)
    t_min = tzmin;
  if (tzmax < t_max)
    t_max = tzmax;
  if (t_min > t_max)
    return false;
  if (t_max < t_min)
    return false;
  return true;
}

}  // namespace

namespace pngd
{

kd_tree::kd_tree(const triangle_surface& s) : surface_ref(s)
{
  std::vector<size_t> indices(s.triangles.size());
  std::iota(std::begin(indices), std::end(indices), 0);
  nodes.reserve(2 * surface_ref.triangles.size());
  build(0, surface_ref.triangles.size(), indices, 0, nodes);
}

bool kd_tree::hit(const ray& r, float t_min, float t_max, intersection& rec)
{
  std::vector<intersection> in{};
  std::vector<size_t> indices;
  indices.reserve(
      static_cast<size_t>(std::log(surface_ref.triangles.size()) * 2.0));
  indices.push_back(nodes.size() - 1);
  const auto l{glm::length(r.direction)};

  while (!indices.empty())
  {
    const auto& n{nodes[indices.back()]};
    indices.pop_back();
    if (n.left == std::numeric_limits<size_t>::max())
    {
      glm::vec3 b;
      double t;
      auto tria = surface_ref.triangles[n.index];
      if (project(r, t, b, surface_ref.vertices[tria[0]],
                  surface_ref.vertices[tria[1]], surface_ref.vertices[tria[2]],
                  t_min, t_max))
      {
        intersection i;
        i.t = t * l;
        i.point = glm::vec3(0, 0, 0);
        for (size_t j{0}; j < 3; ++j)
          i.point =
              i.point +
              b[j] * surface_ref.vertices[surface_ref.triangles[n.index][j]];
        in.push_back(i);
      }
    }
    else
    {
      float tmin, tmax;
      if (project(r, nodes[n.left].bb, tmin, tmax))
        indices.push_back(n.left);
      if (project(r, nodes[n.right].bb, tmin, tmax))
        indices.push_back(n.right);
    }
  }

  return !in.empty();
}

size_t kd_tree::build(size_t f, size_t s, std::vector<size_t>& indices,
                      size_t l, std::vector<kd_node>& nodes)
{
  kd_node n;
  size_t dim{l % 3};
  size_t mid{f + (s - f) / 2};
  std::nth_element(
      std::next(std::begin(indices), f), std::next(std::begin(indices), mid),
      std::next(std::begin(indices), s), [&](const auto i1, const auto i2) {
        return compare(surface_ref, i1, i2, dim);
      });

  if (mid != f)
  {
    n.left = build(f, mid, indices, l + 1, nodes);
    n.right = build(mid, s, indices, l + 1, nodes);
    n.bb = unite(nodes[n.left].bb, nodes[n.right].bb);
  }
  else
  {
    n.left = n.right = std::numeric_limits<size_t>::max();
    n.index = indices[f];
    n.bb = ::make_bbox(surface_ref, indices[f]);
  }
  nodes.push_back(n);

  return nodes.size() - 1;
}

}  // namespace pngd
