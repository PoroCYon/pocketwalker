file(GLOB_RECURSE SOURCES *.cpp *.h)

add_library(pocketwalker_core STATIC ${SOURCES})
target_include_directories(pocketwalker_core PUBLIC ..)