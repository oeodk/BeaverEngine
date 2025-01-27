cmake_minimum_required(VERSION 3.20)
project(SandBox VERSION 1.0 LANGUAGES CXX)

# Collect source and header files
file(GLOB_RECURSE SANDBOX_SOURCES src/*.cpp)
file(GLOB_RECURSE SANDBOX_HEADERS include/*.h)

# Create the executable
add_executable(SandBox ${SANDBOX_SOURCES} ${SANDBOX_HEADERS})

# Include directories
target_include_directories(SandBox PUBLIC include)

# Specify GLM include directory (since it's a header-only library)
target_include_directories(BeaverEngine PRIVATE ${CMAKE_SOURCE_DIR}/_deps/glm-src)

target_include_directories(BeaverEngine PUBLIC ${yaml-cpp_SOURCE_DIR}/include)

# Link with BeaverEngine
target_link_libraries(SandBox PUBLIC BeaverEngine glm yaml-cpp)

# Specify output directories
set_target_properties(SandBox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
