cmake_minimum_required(VERSION 3.20)
project(BeaverEngine VERSION 1.0 LANGUAGES CXX)

# Detect Emscripten (WebAssembly) build
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
    add_compile_definitions("__EMSCRIPTEN__")


    message(STATUS "🔶 Building for WebAssembly with Emscripten")
    # Use Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE C:/Users/gonon/Downloads/emsdk-main/emsdk-main/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
	
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # Optional: If you use SDL for input/audio (instead of GLFW)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=3")

    # Adjust linker behavior
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_CXX_FLAGS}")

    # Disable unsupported Emscripten features
    set(BUILD_SHARED_LIBS OFF)
	 # Skip threads, skip tests/examples for all dependencies
    set(THREADS_FOUND TRUE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
	
	set(BUILDING_WASM TRUE)
	
	option(BEAVER_ENGINE_BUILD_EXAMPLES "Build example projects" OFF)
    option(BEAVER_ENGINE_BUILD_SANDBOX "Build sandbox projects" OFF)
	
	set(LIBTYPE STATIC CACHE STRING "Build static OpenAL library" FORCE)
	set(ALSOFT_UTILS OFF CACHE BOOL "Don't build OpenAL utilities" FORCE)
	set(ALSOFT_EXAMPLES OFF CACHE BOOL "Don't build OpenAL examples" FORCE)
	set(ALSOFT_TESTS OFF CACHE BOOL "Don't build OpenAL tests" FORCE)
else()
    set(BUILDING_WASM FALSE)
	option(BEAVER_ENGINE_BUILD_EXAMPLES "Build example projects" ON)
    option(BEAVER_ENGINE_BUILD_SANDBOX "Build sandbox projects" ON)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILDING_WASM)
	# Define dependency paths
	set(DEPS_DIR ${CMAKE_SOURCE_DIR}/_deps_web)
	set(BUILD_DIR ${DEPS_DIR})
else()
	# Define dependency paths
	set(DEPS_DIR ${CMAKE_SOURCE_DIR}/_deps)
	set(BUILD_DIR ${CMAKE_BINARY_DIR})
endif()




# Add external libraries
include(FetchContent)


if(NOT BUILDING_WASM)
	message(STATUS "🔶 Building GLFW")

	# GLFW
	FetchContent_Declare(
		glfw
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		GIT_TAG master
		SOURCE_DIR ${DEPS_DIR}/glfw-src
		BINARY_DIR ${DEPS_DIR}/glfw-build
	)
	FetchContent_MakeAvailable(glfw)

	message(STATUS "🔶 Building OPENAL")


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

	get_property(OPENAL_TARGETS DIRECTORY ${openal_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
	message(STATUS "OpenAL Targets: ${OPENAL_TARGETS}")
	
	
	set(SDL_SHARED OFF CACHE BOOL "" FORCE)
	set(SDL_STATIC ON CACHE BOOL "" FORCE)
	set(SDL_TEST OFF CACHE BOOL "" FORCE)

	set(SDL_HAPTIC ON CACHE BOOL "" FORCE)
	set(SDL_JOYSTICK ON CACHE BOOL "" FORCE)
	set(SDL_GAMECONTROLLER ON CACHE BOOL "" FORCE)

	# Video must be ON (or at least not fully disabled) on Windows
	set(SDL_VIDEO ON CACHE BOOL "" FORCE)
	set(SDL_RENDER OFF CACHE BOOL "" FORCE)

	set(SDL_EVENTS ON CACHE BOOL "" FORCE)
	set(SDL_TIMERS ON CACHE BOOL "" FORCE)
	set(SDL_THREADS ON CACHE BOOL "" FORCE)

	# Optional: disable subsystems you truly don't need
	set(SDL_POWER OFF CACHE BOOL "" FORCE)
	set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
	set(SDL_FILESYSTEM OFF CACHE BOOL "" FORCE)
	set(SDL_LOADSO OFF CACHE BOOL "" FORCE)
	set(SDL_LOCALE OFF CACHE BOOL "" FORCE)
	set(SDL_MISC OFF CACHE BOOL "" FORCE)

	message(STATUS "🔶 Building SDL3")


	FetchContent_Declare(
		SDL3-static
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG main
		SOURCE_DIR ${DEPS_DIR}/sdl3-src
		BINARY_DIR ${DEPS_DIR}/sdl3-build
	)
	FetchContent_MakeAvailable(SDL3-static)
endif()



message(STATUS "🔶 Building GLM")

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/glm-src
    BINARY_DIR ${DEPS_DIR}/glm-build
)
FetchContent_MakeAvailable(glm)

message(STATUS "🔶 Building YAML-CPP")

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

message(STATUS "🔶 Building nlohmann-json")


# JSON
FetchContent_Declare(
    nlohmann-json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG master
    SOURCE_DIR ${DEPS_DIR}/nlohmann-json-src
    BINARY_DIR ${DEPS_DIR}/nlohmann-json-build
)
FetchContent_MakeAvailable(nlohmann-json)

## freetype
#FetchContent_Declare(
#    freetype
#    GIT_REPOSITORY https://github.com/freetype/freetype.git
#    GIT_TAG master
#    SOURCE_DIR ${DEPS_DIR}/freetype-src
#    BINARY_DIR ${DEPS_DIR}/freetype-build
#)
#FetchContent_MakeAvailable(freetype)
message(STATUS "🔶 Building LIBSNDFILE")
# LIBSNDFILE
set(BUILD_TESTING OFF CACHE BOOL "Disable test for sndfile" FORCE)
set(ENABLE_TESTING OFF CACHE BOOL "Disable testing for sndfile" FORCE)
set(ENABLE_EXTERNAL_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
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


if(NOT BUILDING_WASM)
	# Glad
	add_subdirectory(_deps/glad)
endif()
# Add subdirectories
add_subdirectory(BeaverEngine)

if(BUILDING_WASM)

message(STATUS "🔶CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}🔶")
# linker options only
	target_compile_definitions(BeaverEngine PUBLIC SHIPPING)
    # Emscripten compile-time flags
    target_compile_options(BeaverEngine PRIVATE
        "-sUSE_GLFW=3"
        "-sUSE_SDL=2"
        "-lopenal"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES3=1"
		#"-sASSERTIONS=2"
		#"-gsource-map"
		#"-DEMANGLE_SUPPORT=1"
		#"-g4"
        "-O2"
    )

    # Emscripten link-time flags
    target_link_options(BeaverEngine PRIVATE
        "-sUSE_GLFW=3"
        "-sUSE_SDL=2"
        "-lopenal"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES3=1"
        "-sWASM=1"
		#"-sASSERTIONS=2"
		#"-gsource-map"
		#"-DEMANGLE_SUPPORT=1"
		#"-g4"
        "-O2"
    )
endif()

if(BEAVER_ENGINE_BUILD_EXAMPLES)
	add_subdirectory(examples/BrickBreaker)
	add_subdirectory(examples/WindowPong)
endif()

if(BEAVER_ENGINE_BUILD_SANDBOX)
	add_subdirectory(SandBox)
	
	if(BUILDING_WASM)

    # Emscripten compile-time flags
    target_compile_options(SandBox PRIVATE
        "-sUSE_GLFW=3"
        "-sUSE_SDL=2"
        "-lopenal"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES3=1"
		#"-sASSERTIONS=2"
		#"-gsource-map"
		#"-DEMANGLE_SUPPORT=1"
		#"-g4"
        "-O2"
    )

    # Emscripten link-time flags
    target_link_options(SandBox PRIVATE
        "-sUSE_GLFW=3"
        "-sUSE_SDL=2"
        "-lopenal"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sFULL_ES3=1"
        "-sWASM=1"
		"--preload-file=${CMAKE_SOURCE_DIR}/data@/data"
		#"-sASSERTIONS=2"
		#"-gsource-map"
		#"-DEMANGLE_SUPPORT=1"
		#"-g4"
        "-O2"
    )
	endif()
endif()
