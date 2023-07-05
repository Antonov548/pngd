#include "utils.h"

#include <fstream>

namespace pngd
{

bool save_file(const buffer& buf, const std::string& file_path)
{
  std::ofstream output_file_stream(file_path,
                                   std::ios::binary | std::ios::trunc);

  if (!output_file_stream)
  {
    return false;
  }

  if (!output_file_stream.write(reinterpret_cast<const char*>(buf.data()),
                                static_cast<std::streamsize>(buf.size())))
  {
    return false;
  }

  return true;
}

}  // namespace pngd
