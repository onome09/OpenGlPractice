#pragma once
#include <GL/glew.h>


#include <signal.h>
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"


#define GLCall(x) GLClearError();		\
  x;						\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))
  
#define ASSERT(x) if (!(x)) raise(SIGINT);

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
  void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
  void Clear() const;
  Renderer(){}
  ~Renderer(){}
};


