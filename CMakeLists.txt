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

set(LIBRARY_NAME pngd)
set(TARGET_NAME launcher)

set(ALL_HEADERS
  src/bbox.h
  src/scene.h
  src/camera.h
  src/render.h
  src/utils.h
  src/primitives.h
  src/mesh.h
  src/kdtree.h
  src/trianglesurface.h
  src/lodepng/lodepng.h
)

set(LIBRARY_SOURCES
  src/bbox.cpp
  src/scene.cpp
  src/render.cpp
  src/camera.cpp
  src/utils.cpp
  src/primitives.cpp
  src/mesh.cpp
  src/kdtree.cpp
  src/trianglesurface.cpp
  src/lodepng/lodepng.cpp
)

set(ALL_SOURCES
  ${LIBRARY_SOURCES}
  app/main.cpp
)

add_library(${LIBRARY_NAME} ${LIBRARY_SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${DEPS_INSTALL_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_directories(${LIBRARY_NAME} PUBLIC ${DEPS_INSTALL_DIR}/lib)
target_link_libraries(${LIBRARY_NAME} PUBLIC glm::glm)

add_executable(${TARGET_NAME} app/main.cpp ${ALL_HEADERS})
target_link_libraries(${TARGET_NAME} PUBLIC ${LIBRARY_NAME})

set_target_properties(
    ${TARGET_NAME}
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

add_clang_format_target()
