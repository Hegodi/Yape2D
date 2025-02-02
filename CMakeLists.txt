cmake_minimum_required(VERSION 3.13)
project(Yape2D_Playground)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "source/playground/*.cpp" "source/yape2d/*.cpp")
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

include_directories("source")
include_directories("source/playground")

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")  # Output to HTML for Emscripten
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1") 

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/emscripten)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/emscripten)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/emscripten)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win64)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win64)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win64)
endif()

# Add the executable
add_executable(Yape2D_Playground ${SOURCES})
