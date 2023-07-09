#pragma once

#include "tiny_obj_loader.h"
#include "types.h"

namespace pngd
{

triangle_surface make_surface(const tinyobj::attrib_t& attr,
                              const tinyobj::mesh_t& mesh) noexcept;

}  // namespace pngd
