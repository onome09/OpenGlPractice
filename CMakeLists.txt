project (OpenGlPractice)  #not sure whether this should be the same name of the executable, but I always see that "convention"
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wgnu-array-member-paren-init")

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

find_library(COCOA_LIBRARY Cocoa)
find_library(OPENGL_LIBRARY OpenGl)
find_library(IOKIT_LIBRARY IOKit)
find_library(COREVIDEO_LIBRARY CoreVideo)

ADD_EXECUTABLE(Application ${SRC_FILES})
add_custom_command(TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Application> ${CMAKE_SOURCE_DIR}
)
target_include_directories(Application PUBLIC Dependencies/glfw/include)
target_include_directories(Application PUBLIC Dependencies/glew/include)
target_include_directories(Application PUBLIC ${CMAKE_SOURCE_DIR}/src/vendor/imgui)
target_include_directories(Application PUBLIC ${CMAKE_SOURCE_DIR}/src/tests)
target_include_directories(Application PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Application ${CMAKE_SOURCE_DIR}/Dependencies/glfw/lib/libglfw3.a)
target_link_libraries(Application ${CMAKE_SOURCE_DIR}/Dependencies/glew/lib/libGLEW.a)
target_link_libraries(Application ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
