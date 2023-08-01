#pragma once

#include <memory>

#include "types.h"

namespace pngd
{

struct kd_node
{
  bbox bb;
  size_t index, left, right;
};

struct kd_tree : hittable
{
  explicit kd_tree(const triangle_surface& s);

  const triangle_surface& surface_ref;

  bool hit(const ray& r, float t_min, float t_max, intersection& rec) final;

 private:
  size_t build(size_t f, size_t s, std::vector<size_t>& indices, size_t level,
               std::vector<kd_node>& node);

 private:
  std::vector<kd_node> nodes{};
};

}  // namespace pngd
