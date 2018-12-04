project (OpenGlPractice)  #not sure whether this should be the same name of the executable, but I always see that "convention"
cmake_minimum_required(VERSION 2.8)

find_library(COCOA_LIBRARY Cocoa)
find_library(OPENGL_LIBRARY OpenGl)
find_library(IOKIT_LIBRARY IOKit)
find_library(COREVIDEO_LIBRARY CoreVideo)

ADD_EXECUTABLE(Application src/Application.cpp)
target_include_directories(Application PUBLIC Dependencies/glfw/include)
target_include_directories(Application PUBLIC Dependencies/glew/include)
target_link_libraries(Application ${CMAKE_SOURCE_DIR}/Dependencies/glfw/lib/libglfw3.a)
target_link_libraries(Application ${CMAKE_SOURCE_DIR}/Dependencies/glew/lib/libGLEW.a)
target_link_libraries(Application ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})