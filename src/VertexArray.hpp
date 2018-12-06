#pragma once
#include "VertexBuffer.hpp"

class VertexBufferLayout;
class VertexArray
{
private:
  unsigned int m_RendererID;
  
public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;
  void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};
