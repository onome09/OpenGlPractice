#pragma once
#include <signal.h>
#include <GL/glew.h>

#define GLCall(x) GLClearError();		\
  x;						\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))
  
#define ASSERT(x) if (!(x)) raise(SIGINT);

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

