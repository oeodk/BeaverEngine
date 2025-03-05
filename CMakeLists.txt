cmake_minimum_required(VERSION 3.20)
project(BeaverEngine VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define dependency paths
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/_deps)
set(BUILD_DIR ${CMAKE_BINARY_DIR})

option(BEAVER_ENGINE_BUILD_EXAMPLES "Build example projects" ON)
option(BEAVER_ENGINE_BUILD_SANDBOX "Build sandbox projects" ON)

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

# OPENAL-SOFT
FetchContent_Declare(
    openal
    GIT_REPOSITORY https://github.com/kcat/openal-soft.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/openal-soft-src
    BINARY_DIR ${DEPS_DIR}/openal-soft-build
)

# Disable unnecessary components
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Disable examples in OpenAL Soft")
set(ALSOFT_TESTS OFF CACHE BOOL "Disable tests in OpenAL Soft")
set(ALSOFT_UTILS OFF CACHE BOOL "Disable utilities in OpenAL Soft")
set(ALSOFT_INSTALL OFF CACHE BOOL "Disable installation in OpenAL Soft")
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "Disable installation in OpenAL Soft")

# Enable required components
set(OPENAL_BUILD_STATIC_LIBS ON CACHE BOOL "Build static libraries for OpenAL")
set(OPENAL_BUILD_SHARED_LIBS OFF CACHE BOOL "Disable shared libraries for OpenAL")

# Build only the core components
set(OPENAL_BUILD_COMMON ON CACHE BOOL "Build the common components")
set(OPENAL_BUILD_VERSION ON CACHE BOOL "Build the versioning components")

FetchContent_MakeAvailable(openal)

# LIBSNDFILE
FetchContent_Declare(
    sndfile
    GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/sndfile-src
    BINARY_DIR ${DEPS_DIR}/sndfile-build
)
set(BUILD_PROGRAMS OFF CACHE BOOL "Disable building command-line programs for libsndfile")
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable building example programs for libsndfile")
set(BUILD_TESTING OFF CACHE BOOL "Disable building testing for libsndfile")

FetchContent_MakeAvailable(sndfile)

get_property(OPENAL_TARGETS DIRECTORY ${openal_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
message(STATUS "OpenAL Targets: ${OPENAL_TARGETS}")

# JSON
FetchContent_Declare(
    nlohmann-json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/nlohmann-json-src
    BINARY_DIR ${DEPS_DIR}/nlohmann-json-build
)
FetchContent_MakeAvailable(nlohmann-json)

# Define the base output directory
set(BASE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

# Define intermediate and binary output paths for different configurations
set(INTERM_OUTPUT_DIR ${BASE_OUTPUT_DIR}/interm/${PROJECT_NAME}/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})
set(OUT_OUTPUT_DIR ${BASE_OUTPUT_DIR}/out/${PROJECT_NAME}/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})

# Create the intermediate directories (this can also be done in CMake's build process, but manually specifying gives flexibility)
file(MAKE_DIRECTORY ${INTERM_OUTPUT_DIR})
file(MAKE_DIRECTORY ${OUT_OUTPUT_DIR})

# Set the output directory for static libraries (INTERMEDIATE files)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${INTERM_OUTPUT_DIR}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_OUTPUT_DIR}/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_OUTPUT_DIR}/$<CONFIG>)

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

# Set linker flags for each configuration type
set(CMAKE_CXX_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD")
set(CMAKE_C_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD")
set(CMAKE_EXE_LINKER_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD")
set(CMAKE_SHARED_LINKER_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD" CACHE STRING "Linker flags for shipping configuration")

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

if(BEAVER_ENGINE_BUILD_EXAMPLES)
	add_subdirectory(examples/BrickBreaker)
	add_subdirectory(examples/WindowPong)
endif()

if(BEAVER_ENGINE_BUILD_SANDBOX)
	add_subdirectory(SandBox)
endif()