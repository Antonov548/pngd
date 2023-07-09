#include "trianglesurface.h"

#include <algorithm>

#include "types.h"

namespace pngd
{

triangle_surface make_surface(const tinyobj::attrib_t& attr,
                              const tinyobj::mesh_t& mesh) noexcept
{
  triangle_surface surface{};
  surface.triangles.resize(mesh.num_face_vertices.size());

  std::map<size_t, size_t> vm;

  for (size_t i{0}; i < mesh.num_face_vertices.size(); ++i)
  {
    triangle t{};

    for (size_t j{0}; j < 3; ++j)
    {
      const auto vertex_idx{mesh.indices[i + j].vertex_index};
      const auto it_idx{vm.find(vertex_idx)};

      if (it_idx == std::end(vm))
      {
        surface.vertices.push_back(glm::vec3{
            attr.vertices[vertex_idx * 3], attr.vertices[vertex_idx * 3 + 1],
            attr.vertices[vertex_idx * 3 + 2]});

        vm[vertex_idx] = surface.vertices.size() - 1;
        t[j] = vm[vertex_idx];
      }
      else
      {
        t[j] = it_idx->second;
      }
    }

    surface.triangles[i] = t;
  }

  return surface;
}

}  // namespace pngd
