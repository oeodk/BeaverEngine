cmake_minimum_required(VERSION 3.20)
project(WindowPong VERSION 1.0 LANGUAGES CXX)

# Collect source and header files
file(GLOB_RECURSE PONG_SOURCES src/*.cpp)
file(GLOB_RECURSE PONG_HEADERS include/*.h)

# Create the executable
add_executable(WindowPong ${PONG_SOURCES} ${PONG_HEADERS})

# Include directories
target_include_directories(WindowPong PUBLIC include)

# Specify GLM include directory (since it's a header-only library)
target_include_directories(BeaverEngine PRIVATE ${CMAKE_SOURCE_DIR}/_deps/glm-src)

target_include_directories(WindowPong PUBLIC ${yaml-cpp_SOURCE_DIR}/include)

target_include_directories(WindowPong PUBLIC ${CMAKE_SOURCE_DIR}/_deps/nlohmann-json-src/include)

# Link with BeaverEngine
target_link_libraries(WindowPong PUBLIC BeaverEngine glm yaml-cpp)

# Specify output directories
set_target_properties(WindowPong PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
