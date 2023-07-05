#include "primitives.h"
#include "render.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
  const auto aspect_ratio = 16.0 / 9.0;
  const int width = 400;
  const int height = static_cast<int>(width / aspect_ratio);

  pngd::scene s{.width = width, .height = height};
  s.objects.push_back(std::make_unique<pngd::sphere>(glm::vec3(0, 0, -1), 0.5));

  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = glm::vec3(0, 0, 0);
  auto horizontal = glm::vec3(viewport_width, 0, 0);
  auto vertical = glm::vec3(0, viewport_height, 0);
  auto lower_left_corner = origin - horizontal / 2.f - vertical / 2.f -
                           glm::vec3(0, 0, focal_length);

  pngd::camera c{};
  c.position = origin;
  c.vertical = vertical;
  c.horizontal = horizontal;
  c.lower_left_corner = lower_left_corner;

  const auto png{pngd::render_to_png(s, c, 1)};

  pngd::save_file(png, "/home/antonov548/pngd/build/image.png");

  return 0;
}
