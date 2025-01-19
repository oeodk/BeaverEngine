cmake_minimum_required(VERSION 3.20)
project(BeaverEngine VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define dependency paths
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/_deps)
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# Add external libraries
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/glfw-src
    BINARY_DIR ${DEPS_DIR}/glfw-build
)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/glm-src
    BINARY_DIR ${DEPS_DIR}/glm-build
)
FetchContent_MakeAvailable(glm)

# YAML-CPP
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/yaml-cpp-src
    BINARY_DIR ${DEPS_DIR}/yaml-cpp-build
)
# Disable contrib and tools
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Disable contrib in yaml-cpp")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Disable tools in yaml-cpp")

# Configure yaml-cpp to build only the static library (without contrib and tools)
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library for yaml-cpp")


FetchContent_MakeAvailable(yaml-cpp)

# Define the base output directory
set(BASE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

# Define intermediate and binary output paths for different configurations
set(INTERM_OUTPUT_DIR ${BASE_OUTPUT_DIR}/interm/${PROJECT_NAME}/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})
set(OUT_OUTPUT_DIR ${BASE_OUTPUT_DIR}/out/${PROJECT_NAME}/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})

# Create the intermediate directories (this can also be done in CMake's build process, but manually specifying gives flexibility)
file(MAKE_DIRECTORY ${INTERM_OUTPUT_DIR})
file(MAKE_DIRECTORY ${OUT_OUTPUT_DIR})

# Set the output directory for static libraries (INTERMEDIATE files)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${INTERM_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${INTERM_OUTPUT_DIR}/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${INTERM_OUTPUT_DIR}/debug)

# Set the output directory for shared libraries (if you have them)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT_OUTPUT_DIR}/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUT_OUTPUT_DIR}/debug)

# Set the output directory for executables (FINAL binaries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_OUTPUT_DIR}/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUT_OUTPUT_DIR}/debug)

# Set the working directory for the executable (at runtime)
set(DATA_SOURCE_DIR ${CMAKE_SOURCE_DIR}/data)

# Define custom configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release Shipping CACHE STRING "Available build types" FORCE)

# Define flags for the Shipping configuration
set(CMAKE_CXX_FLAGS_SHIPPING "-O3 -DNDEBUG -DSHIPPING_BUILD")
set(CMAKE_C_FLAGS_SHIPPING "-O3 -DNDEBUG -DSHIPPING_BUILD")
set(CMAKE_EXE_LINKER_FLAGS_SHIPPING "")
set(CMAKE_SHARED_LINKER_FLAGS_SHIPPING "")

# Set global definitions for all configurations
add_compile_definitions(
    $<$<CONFIG:Shipping>:SHIPPING>
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

# Glad
add_subdirectory(_deps/glad)

# Add subdirectories
add_subdirectory(BeaverEngine)
add_subdirectory(SandBox)
