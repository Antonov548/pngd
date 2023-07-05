#pragma once

#include <memory>

#include "types.h"

namespace pngd
{

struct scene
{
  uint32_t width;
  uint32_t height;

  std::vector<std::unique_ptr<hittable>> objects;
};

}  // namespace pngd
