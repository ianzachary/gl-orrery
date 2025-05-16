message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(glad)
find_package(glfw3)
find_package(glm)
find_package(stb)

set(CONANDEPS_LEGACY  glad::glad  glfw  glm::glm  stb::stb )