#include "cxxopts.hpp"
#include "primitives.h"
#include "render.h"
#include "scene.h"
#include "utils.h"

int main(int argc, char const* argv[])
{

  try
  {
    cxxopts::Options options{argv[0]};

    // clang-format off
    options.add_options()
      ("w,width", "Image width", cxxopts::value<uint32_t>())
      ("h,height", "Image height", cxxopts::value<uint32_t>())
      ("o,output", "Output PNG path", cxxopts::value<std::string>());
    // clang-format on

    const auto args{options.parse(argc, argv)};

    const auto width{args["width"].as<uint32_t>()};
    const auto height{args["height"].as<uint32_t>()};

    const auto output_file{args["output"].as<std::string>()};

    const auto aspect_ratio{static_cast<float>(width) /
                            static_cast<float>(height)};

    pngd::scene s{.width = width, .height = height};

    s.objects.push_back(
        std::make_unique<pngd::sphere>(glm::vec3{0, 0, -1}, 0.5));
    s.objects.push_back(
        std::make_unique<pngd::sphere>(glm::vec3{1, 0, -1}, 0.7));

    glm::vec3 origin{0, 0, 2};
    glm::vec3 direction{0, 0, -1};
    glm::vec3 up{1, 0, 0};

    const auto c{pngd::make_camera(origin, direction, up, 45, aspect_ratio)};

    const auto png{pngd::render_to_png(s, c, 1)};
    pngd::save_file(png, output_file);

    return 0;
  }
  catch (const cxxopts::exceptions::exception& e)
  {
    return 1;
  }
}
