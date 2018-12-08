#include <GL/glew.h>
#include "TestClearColor.hpp"
#include "TestTexture.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/glm.hpp"
#include "Texture.hpp"
#include <iostream>
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Renderer.hpp"

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

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  
 
  Renderer renderer;

  ImGui::CreateContext();
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();
  

  test::Test* currentTest = nullptr;
  test::TestMenu* testMenu = new test::TestMenu(currentTest);

  currentTest = testMenu;

  testMenu->RegisterTest<test::TestClearColor>("Clear Color");
  testMenu->RegisterTest<test::TestTexture>("Texture");

  while (!glfwWindowShouldClose(window))
    {
      GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
      renderer.Clear();

      ImGui_ImplGlfwGL3_NewFrame();
      if (currentTest) {

	currentTest->OnUpdate(0.0f);
	currentTest->OnRender();
	ImGui::Begin("Test");
	if (currentTest != testMenu && ImGui::Button("<-")){
	  delete currentTest;
	  currentTest = testMenu;
	}
	currentTest->OnImGuiRender();
	ImGui::End();
      }
      ImGui::Render();
      ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  delete currentTest;

  if (currentTest != testMenu){
    delete testMenu;
  }
  ImGui_ImplGlfwGL3_Shutdown();
  ImGui::DestroyContext();

  //  GLCall(glDeleteProgram(shader));
  glfwTerminate();
  return 0;
}
