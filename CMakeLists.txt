cmake_minimum_required(VERSION 3.10)
project(heap-scanner)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(
        "-Wall" "-Wpedantic" "-Wextra" "-fexceptions" "-Wno-unused-parameter"
        "$<$<CONFIG:DEBUG>:-fsanitize=address>"
)

add_link_options(
        "$<$<CONFIG:DEBUG>:-fsanitize=address>"
)


string(TOLOWER ${CMAKE_SYSTEM_NAME} OS)
include_directories($ENV{JDK_DIR}/include/${OS}/)
include_directories($ENV{JDK_DIR}/include/)
include_directories(src/main/cpp)
include_directories(src/main/cpp/vm)
include_directories(src/main/cpp/vm/metadata)
include_directories(src/main/cpp/vm/symbols)
include_directories(src/main/cpp/vm/utilities)
include_directories(src/main/cpp/vm/models)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/main/cpp/*.cpp)

link_directories($ENV{JDK_DIR}/lib/server/)
link_libraries(jvm)

add_library(agent SHARED ${SOURCES} src/main/cpp/vm/symbols/symbolsParser_linux.cpp)