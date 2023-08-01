#include "bbox.h"

namespace pngd
{

bbox make_bbox(const glm::vec3& f, const glm::vec3 s) noexcept
{
  bbox bb;

  for (size_t i{0}; i < 3; ++i)
  {
    if (f[i] > s[i])
    {
      bb.second[i] = f[i];
      bb.first[i] = s[i];
    }
    else
    {
      bb.first[i] = f[i];
      bb.second[i] = s[i];
    }
  }

  return bb;
}

bbox unite(const bbox& bb, const glm::vec3& p) noexcept
{
  bbox result{bb};

  if (result.first[0] > p[0])
    result.first[0] = p[0];
  if (result.second[0] < p[0])
    result.second[0] = p[0];

  if (result.first[1] > p[1])
    result.first[1] = p[1];
  if (result.second[1] < p[1])
    result.second[1] = p[1];

  if (result.first[2] > p[2])
    result.first[2] = p[2];
  if (result.second[2] < p[2])
    result.second[2] = p[2];

  return result;
}

bbox unite(const bbox& l, const bbox& r) noexcept
{
  bbox result{l};

  if (result.first[0] > r.first[0])
    result.first[0] = r.first[0];
  if (result.second[0] < r.second[0])
    result.second[0] = r.second[0];

  if (result.first[1] > r.first[1])
    result.first[1] = r.first[1];
  if (result.second[1] < r.second[1])
    result.second[1] = r.second[1];

  if (result.first[2] > r.first[2])
    result.first[2] = r.first[2];
  if (result.second[2] < r.second[2])
    result.second[2] = r.second[2];

  return result;
}

}  // namespace pngd
