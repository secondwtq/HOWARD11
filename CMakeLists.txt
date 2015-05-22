cmake_minimum_required(VERSION 3.0)
project(HOWARD11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(HOWARD11 ${SOURCE_FILES} Entry.cpp Debug.hxx)

target_link_libraries(HOWARD11 glfw3)

if(APPLE)

    find_library(FRAMEWORK_COCOA Cocoa)
    find_library(FRAMEWORK_GL OpenGL)
    find_library(FRAMEWORK_IOKIT IOKit)
    find_library(FRAMEWORK_CORE_VIDEO CoreVideo)
    # mark_as_advanced(FRAMEWORK_COCOA)
    set(EXTRA_LIBS_APPLE ${FRAMEWORK_COCOA} ${FRAMEWORK_GL} ${FRAMEWORK_IOKIT}
        ${FRAMEWORK_CORE_VIDEO})

endif(APPLE)

target_link_libraries(HOWARD11 ${EXTRA_LIBS_APPLE})