#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "lodepng/lodepng.h"

static const struct
{
  float x, y;
  float r, g, b;
} vertices[3] = {{-0.6f, -0.4f, 1.f, 0.f, 0.f},
                 {0.6f, -0.4f, 0.f, 1.f, 0.f},
                 {0.f, 0.6f, 0.f, 0.f, 1.f}};

static const char* vertex_shader_text =
    "#version 110\n"
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec2 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char* fragment_shader_text =
    "#version 110\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

int main(int argc, char* argv[])
{
  if (!glfwInit())
  {
    spdlog::error("GLFW initialization failed.");
    return 0;
  }

  // glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  auto* window{glfwCreateWindow(640, 480, "Image creator", NULL, NULL)};
  if (!window)
  {
    spdlog::error("Failed to create GLFW Window.");
    glfwTerminate();

    return 0;
  }

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  glClearColor(255, 0, 0, 255);

  GLuint vertex_buffer, vertex_shader, fragment_shader, program;
  GLint mvp_location, vpos_location, vcol_location;

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
  glCompileShader(fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  mvp_location = glGetUniformLocation(program, "MVP");
  vpos_location = glGetAttribLocation(program, "vPos");
  vcol_location = glGetAttribLocation(program, "vCol");

  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                        sizeof(vertices[0]), (void*)0);
  glEnableVertexAttribArray(vcol_location);
  glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertices[0]), (void*)(sizeof(float) * 2));

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);

  float ratio = width / (float)height;
  const auto p{glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f)};
  const auto m{glm::mat4x4(1.0)};
  const auto mvp{m * p};

  glUseProgram(program);
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glfwSwapBuffers(window);

  std::vector<std::byte> pixels{};
  pixels.resize(width * height * 3);

  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

  std::vector<unsigned char> png_data{};
  for (size_t i{0}; i < height; ++i)
  {
    for (size_t j{0}; j < width; ++j)
    {
      const auto index{((height - i - 1) * width + j) * 3};
      png_data.push_back(static_cast<unsigned char>(pixels[index]));
      png_data.push_back(static_cast<unsigned char>(pixels[index + 1]));
      png_data.push_back(static_cast<unsigned char>(pixels[index + 2]));
      png_data.push_back(static_cast<unsigned char>(255));
    }
  }

  std::vector<unsigned char> buffer{};
  auto error{lodepng::encode(buffer, png_data, width, height)};

  if (!error)
    error = lodepng::save_file(buffer, "C:\\image-creator\\image.png");

  if (error)
  {
    spdlog::error("Failed to encode and save PNG file with error: {0}.",
                  lodepng_error_text(error));
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 1;
}