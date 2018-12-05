#include <GL/glew.h>
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Renderer.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath){
  std::ifstream stream(filePath);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  
  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE; 
  while (getline(stream, line)){
    if (line.find("#shader") != std::string::npos){
      if (line.find("vertex") != std::string::npos){
	//Set mode to vertex
	type = ShaderType::VERTEX;
      } else if (line.find("fragment")){
	//Set mode to fragment
	type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << '\n';

    }

  }

  return { ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(const std::string& source, unsigned int type){
  GLCall(unsigned int id = glCreateShader(type)); 
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  //TODO: error handling
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

  if (result == GL_FALSE){
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*) alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile "
	      << (type == GL_VERTEX_SHADER ? "vertex" : "shader")
	      << " shader!" << std::endl;
    std::cout << message << std::endl;

    GLCall(glDeleteShader(id));
    return 0;

    
  }
  
  return id;
}

// strings are the source code of the vertex shader and fragment shader
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  GLCall(unsigned int program = glCreateProgram());

  unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
  unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));
  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  
  return program;
}

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

  unsigned int vao;
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  VertexBuffer vb(positions, 4 * 2 * sizeof(float));
  
  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
  //index 0 of this vertex array is bound to the currently bound GL_ARRAY buffer

  IndexBuffer ib(indices, 6);
  
  ShaderProgramSource source = ParseShader("./res/shaders/Basic.glsl");
  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

  
  GLCall(glUseProgram(shader));

  GLCall(int location = glGetUniformLocation(shader, "u_Color"));
  ASSERT(location != -1);

  GLCall(glUniform4f(location, 0.7f, 0.3f, 0.9f, 1.0f));

  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  
  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {

      /* Render here */
      GLCall(glClear(GL_COLOR_BUFFER_BIT));

      glUseProgram(shader);
      GLCall(glUniform4f(location, r, 0.3f, 0.9f, 1.0f));
      
      GLCall(glBindVertexArray(vao));
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
