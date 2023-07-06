#include "render.h"

#include <algorithm>
#include <random>

#include "lodepng/lodepng.h"

namespace pngd
{

glm::vec3 ray_color(const ray& r, const scene& s)
{
  intersection rec;
  intersection temp_rec;
  bool hit_anything = false;
  auto closest_so_far = std::numeric_limits<float>::max();

  for (const auto& object : s.objects)
  {
    if (object->hit(r, 0.001, closest_so_far, temp_rec))
    {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  if (hit_anything)
  {
    return glm::vec3{1, 0, 0};
  }

  auto unit_direction = glm::normalize(r.direction);
  float t = 0.5 * (unit_direction.y + 1.0);
  return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
}

buffer render_to_png(const scene& s, const camera& c, uint32_t samples)
{
  bitmap pixels;
  pixels.reserve(s.width * s.height * 4);

  for (uint32_t i{0}; i < s.height; ++i)
  {
    for (uint32_t j{0}; j < s.width; ++j)
    {
      glm::vec3 color(0.f, 0.f, 0.f);

      for (uint32_t k{0}; k < samples; ++k)
      {
        const auto u{static_cast<float>(j) / (s.width - 1)};
        const auto v{static_cast<float>(s.height - i) / (s.height - 1)};

        ray r{c.position, c.lower_left_corner + u * c.horizontal +
                              v * c.vertical - c.position};

        color = ray_color(r, s);
      }

      pixels.push_back(color.r * 255.f);
      pixels.push_back(color.y * 255.f);
      pixels.push_back(color.z * 255.f);
      pixels.push_back(255);
    }
  }

  std::vector<unsigned char> png_image{};
  lodepng::encode(png_image, pixels, s.width, s.height);

  buffer buf{};
  buf.reserve(png_image.size());

  std::transform(
      std::begin(png_image), std::end(png_image), std::back_inserter(buf),
      [](const auto value) { return static_cast<std::byte>(value); });

  return std::move(buf);
}

}  // namespace pngd
