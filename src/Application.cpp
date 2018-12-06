#include <GL/glew.h>
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Renderer.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>





int main(void)
{

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
    {
      std::cout << "HI" << std::endl;
      glfwTerminate();
      return -1;
    }
  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK){
    std::cout << "Error: Issue with glewInit ---- exiting" << std::endl;
    return -1;
  }
  std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
    -0.5f, -0.5f, //0
     0.5f, -0.5f, //1
     0.5f,  0.5f, //2
    -0.5f,  0.5f  //3
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
  };

  VertexArray va;

  VertexBuffer vb(positions, 4 * 2 * sizeof(float));
  VertexBufferLayout layout;
  layout.PushFloat(2);
  va.addBuffer(vb, layout);

  IndexBuffer ib(indices, 6);
  Shader shader("./res/shaders/Basic.glsl");
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  
  va.Unbind();
  shader.Unbind();
  vb.Unbind();
  ib.Unbind();
  
  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {

      /* Render here */
      GLCall(glClear(GL_COLOR_BUFFER_BIT));

      shader.Bind();
      shader.SetUniform4f("u_Color", r, 0.3f, 0.3f, 1.0f);
      
      va.Bind();
      ib.Bind();

      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

      if (r > 1.0f){
	increment = -0.05f;
      } else if (r < 0.0f) {
	increment = 0.05f;
      }
      r += increment;
      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  //  GLCall(glDeleteProgram(shader));
  glfwTerminate();
  return 0;
}
