#pragma once

#include <string>

#include "types.h"

namespace pngd
{

bool save_file(const buffer& buf, const std::string& file_path);

}  // namespace pngd
