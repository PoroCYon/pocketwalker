cmake_minimum_required(VERSION 4.0)
project(pocketwalker)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(${CMAKE_SOURCE_DIR}/../core core)
add_subdirectory(${CMAKE_SOURCE_DIR}/../external/argparse argparse)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

if(WIN32)
    set(ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/app.rc")
endif()

add_executable(pocketwalker ${SOURCES} ${ICON_RESOURCE})
set_target_properties(pocketwalker PROPERTIES
        OUTPUT_NAME "pocketwalker"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/release"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/release"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/release"
)
target_include_directories(pocketwalker PRIVATE ${CMAKE_SOURCE_DIR}/../external/argparse/include)
target_link_libraries(pocketwalker PRIVATE pocketwalker_core stdc++exp)