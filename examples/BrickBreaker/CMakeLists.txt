cmake_minimum_required(VERSION 3.20)
project(BrickBreaker VERSION 1.0 LANGUAGES CXX)

# Collect source and header files
file(GLOB_RECURSE BRICK_BREAKER_SOURCES src/*.cpp)
file(GLOB_RECURSE BRICK_BREAKER_HEADERS include/*.h)

# Create the executable
add_executable(BrickBreaker ${BRICK_BREAKER_SOURCES} ${BRICK_BREAKER_HEADERS})

# Include directories
target_include_directories(BrickBreaker PUBLIC include)

# Specify GLM include directory (since it's a header-only library)
target_include_directories(BeaverEngine PRIVATE ${CMAKE_SOURCE_DIR}/_deps/glm-src)

target_include_directories(BrickBreaker PUBLIC ${yaml-cpp_SOURCE_DIR}/include)

target_include_directories(BrickBreaker PUBLIC ${CMAKE_SOURCE_DIR}/_deps/nlohmann-json-src/include)

# Link with BeaverEngine
target_link_libraries(BrickBreaker PUBLIC BeaverEngine glm yaml-cpp)

# Specify output directories
set_target_properties(BrickBreaker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
