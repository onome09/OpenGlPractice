#pragma once
#include "test.hpp"
#include <memory>
#include "Shader.hpp"
#include "vendor/glm/glm.hpp"
#include "Texture.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexBuffer.hpp"

namespace test {
  class TestTexture : public Test {
  
  public:
    TestTexture();
    ~TestTexture();
  
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    glm::vec3 m_TranslationA, m_TranslationB;
    glm::mat4 m_Proj, m_View;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;
  };

  
}
