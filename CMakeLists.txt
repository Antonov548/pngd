cmake_minimum_required(VERSION 3.14)

project(pngd)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(Misc)
include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "bf71a834948186f4097caa076cd2663c69a10e1e"
)

FetchContent_MakeAvailable(glm)

set(TARGET_NAME pngd)

set(ALL_HEADERS
  src/scene.h
  src/camera.h
  src/render.h
  src/utils.h
  src/primitives.h
  src/lodepng/lodepng.h
)

set(ALL_SOURCES
  src/main.cpp
  src/scene.cpp
  src/render.cpp
  src/camera.cpp
  src/utils.cpp
  src/primitives.cpp
  src/lodepng/lodepng.cpp
)

add_executable(${TARGET_NAME} ${ALL_SOURCES} ${ALL_HEADERS})
target_include_directories(${TARGET_NAME} PUBLIC ${DEPS_INSTALL_DIR}/include)

target_link_directories(${TARGET_NAME} PUBLIC ${DEPS_INSTALL_DIR}/lib)
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)

set_target_properties(
    ${TARGET_NAME}
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

add_clang_format_target()
