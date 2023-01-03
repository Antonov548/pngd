cmake_minimum_required(VERSION 3.14)

project(image-creator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(Misc)
include(ExternalProject)
include(FetchContent)

set(DEPS_PROJECTS spdlog_project glfw_project glad_project)
set(DEPS_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdParty)

set(TARGET_NAME imagec)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "bf71a834948186f4097caa076cd2663c69a10e1e"
)

ExternalProject_Add(
    glfw_project
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "3.3.8"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
        -DGLFW_BUILD_EXAMPLES=ON
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
    TEST_COMMAND ""
)

ExternalProject_Add(
    glad_project
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG "v0.1.36"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    PREFIX ${DEPS_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
        -DGLAD_INSTALL=ON
    TEST_COMMAND ""
)

ExternalProject_Add(
    spdlog_project
    GIT_REPOSITORY "https://github.com/gabime/spdlog"
    GIT_TAG "v1.x"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    PREFIX ${DEPS_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
        -DSPDLOG_BUILD_EXAMPLES=OFF
        -DSPDLOG_BUILD_BENCH=OFF
        -DSPDLOG_BUILD_TESTS=OFF
    TEST_COMMAND ""
)

set(TARGET_LIBRARIES glad glfw3)

set(ALL_HEADERS
  src/lodepng/lodepng.h
)

set(ALL_SOURCES
  src/main.cpp
  src/lodepng/lodepng.cpp
)

add_executable(${TARGET_NAME} ${ALL_SOURCES} ${ALL_HEADERS})
add_dependencies(${TARGET_NAME} ${DEPS_PROJECTS})
target_include_directories(${TARGET_NAME} PUBLIC ${DEPS_INSTALL_DIR}/include ${glm_SOURCE_DIR})

target_link_directories(${TARGET_NAME} PUBLIC ${DEPS_INSTALL_DIR}/lib)
target_link_libraries(${TARGET_NAME} PUBLIC ${TARGET_LIBRARIES})

set_target_properties(
    ${TARGET_NAME}
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

add_clang_format_target()