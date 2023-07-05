#pragma once

#include "camera.h"
#include "scene.h"
#include "types.h"

namespace pngd
{

buffer render_to_png(const scene& s, const camera& c, uint32_t samples = 10);

}  // namespace pngd
