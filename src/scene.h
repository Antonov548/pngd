#pragma once

#include <filesystem>
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

bool add_obj(const std::filesystem::path& path, scene& s);

}  // namespace pngd
