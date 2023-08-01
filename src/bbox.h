#pragma once

#include "types.h"

namespace pngd
{

bbox make_bbox(const glm::vec3& f, const glm::vec3 s) noexcept;

bbox unite(const bbox& bb, const glm::vec3& p) noexcept;

bbox unite(const bbox& l, const bbox& r) noexcept;

}  // namespace pngd
