cmake_minimum_required(VERSION 3.20)
project(BeaverEngine VERSION 1.0 LANGUAGES CXX)

# Collect source and header files
file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS include/*.h)

foreach(FILE_PATH ${ENGINE_SOURCES})
    # Get the relative path of the file to the src directory
    file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/src" ${FILE_PATH})

    # Normalize the path to remove unwanted prefixes like "../"
    string(REPLACE "../BeaverEngine/" "/" REL_PATH "${REL_PATH}")

    # Extract the folder hierarchy (relative path without filename)
    get_filename_component(FOLDER_PATH "${REL_PATH}" PATH)

    # Create filters matching the folder hierarchy
    #if(FOLDER_PATH AND NOT FOLDER_PATH MATCHES "^\\.\\.")
        source_group("${FOLDER_PATH}" FILES ${FILE_PATH})
    #else()
    #    source_group("Root" FILES ${FILE_PATH})
    #endif()
endforeach()

foreach(FILE_PATH ${ENGINE_HEADERS})
    # Get the relative path of the file to the include directory
    file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/include" ${FILE_PATH})

    # Normalize the path to remove unwanted prefixes like "../"
    string(REPLACE "../BeaverEngine/" "/" REL_PATH "${REL_PATH}")

    # Extract the folder hierarchy (relative path without filename)
    get_filename_component(FOLDER_PATH "${REL_PATH}" PATH)

    # Create filters matching the folder hierarchy
   # if(FOLDER_PATH AND NOT FOLDER_PATH MATCHES "^\\.\\.")
        source_group("${FOLDER_PATH}" FILES ${FILE_PATH})
    #else()
     #   source_group("Root" FILES ${FILE_PATH})
    #endif()
endforeach()

# Create the library
add_library(BeaverEngine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Include directories
target_include_directories(BeaverEngine PUBLIC include)

# Include the yaml-cpp headers
target_include_directories(BeaverEngine PUBLIC ${yaml-cpp_SOURCE_DIR}/include)
# Specify GLM include directory (since it's a header-only library)
target_include_directories(BeaverEngine PRIVATE ${CMAKE_SOURCE_DIR}/_deps/glm-src)

# Specify GLFW include directory (assuming GLFW is built in the correct path)
target_include_directories(BeaverEngine PRIVATE ${CMAKE_SOURCE_DIR}/_deps/glfw-src/include)

target_include_directories(BeaverEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/_deps/glad/include)

target_include_directories(BeaverEngine PRIVATE ${CMAKE_SOURCE_DIR}/_deps/openal-soft-src/include)

target_include_directories(BeaverEngine PRIVATE ${CMAKE_SOURCE_DIR}/_deps/libsndfile-src/include)

target_include_directories(BeaverEngine PUBLIC ${CMAKE_SOURCE_DIR}/_deps/nlohmann-json-src/include)

# Link external libraries
target_link_libraries(BeaverEngine
    PUBLIC
        glfw
        glm
        yaml-cpp
		glad
		OpenAL::OpenAL
		sndfile
)

# Specify output directories
set_target_properties(BeaverEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
