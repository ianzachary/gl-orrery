include(${CMAKE_TOOLCHAIN_FILE})
cmake_minimum_required(VERSION 3.5)
project(Orrery)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

find_package(OpenGL REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(stb REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    include
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    opengl::opengl
    glad::glad
    glfw
    glm::glm
    stb::stb
)