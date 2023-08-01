#include "render.h"

#include <algorithm>
#include <iostream>
#include <random>

#include "lodepng/lodepng.h"

namespace pngd
{

glm::vec3 ray_color(const ray& r, const scene& s)
{
  intersection in;
  intersection temp_in;

  bool hit_anything = false;
  auto closest_so_far = std::numeric_limits<float>::max();

  for (const auto& object : s.objects)
  {
    if (object->hit(r, 0.001, closest_so_far, temp_in))
    {
      hit_anything = true;
      closest_so_far = temp_in.t;
      in = temp_in;
    }
  }

  if (hit_anything)
  {
    return glm::vec3{1, 0, 0};
  }

  auto unit_direction = glm::normalize(r.direction);
  float t = 0.5 * (unit_direction.y + 1.0);
  return glm::vec3(1, 1, 1);
}

buffer render_to_png(const scene& s, const camera& c, uint32_t samples)
{
  bitmap pixels;
  pixels.reserve(s.width * s.height * 4);

  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_real_distribution<float> rand(0, 1.f);

  for (uint32_t i{0}; i < s.height; ++i)
  {
    for (uint32_t j{0}; j < s.width; ++j)
    {
      glm::vec3 color(0.f, 0.f, 0.f);

      for (uint32_t k{0}; k < samples; ++k)
      {
        const auto u{static_cast<float>(j + rand(gen)) / (s.width - 1)};
        const auto v{static_cast<float>(s.height - i - rand(gen)) /
                     (s.height - 1)};

        ray r{c.position, c.lower_left_corner + u * c.horizontal +
                              v * c.vertical - c.position};

        color += ray_color(r, s);
      }

      pixels.push_back(color.r * 255.f * 1.f / samples);
      pixels.push_back(color.y * 255.f * 1.f / samples);
      pixels.push_back(color.z * 255.f * 1.f / samples);
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
