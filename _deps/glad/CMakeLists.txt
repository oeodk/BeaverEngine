# Specify the include directory for Glad
include_directories(include)

# Collect source files for Glad (Assuming `glad.c` is inside the `src` directory)
file(GLOB_RECURSE GLAD_SOURCES src/*.c)

# Create a static library for Glad (explicitly specifying C language)
add_library(glad STATIC ${GLAD_SOURCES})

# Set the language to C (since Glad is written in C)
set_target_properties(glad PROPERTIES
    LINKER_LANGUAGE C
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
