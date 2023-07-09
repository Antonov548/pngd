#include "scene.h"

#include "mesh.h"
#include "trianglesurface.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace pngd
{

bool add_obj(const std::filesystem::path& path, scene& s)
{
  tinyobj::attrib_t attr;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  if (!tinyobj::LoadObj(&attr, &shapes, &materials, &err, path.c_str()))
    return false;

  for (const auto& shape : shapes)
  {
    const auto surface{make_surface(attr, shape.mesh)};
    s.objects.push_back(std::make_unique<mesh>(std::move(surface)));
  }

  return true;
}

}  // namespace pngd
