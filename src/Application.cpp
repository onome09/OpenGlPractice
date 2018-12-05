#include <GL/glew.h>
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
  unsigned int id = glCreateShader(type); 
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  //TODO: error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE){
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*) alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile "
	      << (type == GL_VERTEX_SHADER ? "vertex" : "shader")
	      << " shader!" << std::endl;
    std::cout << message << std::endl;

    glDeleteShader(id);
    return 0;

    
  }
  
  return id;
}

// strings are the source code of the vertex shader and fragment shader
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  unsigned int program = glCreateProgram();

  unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
  unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);


  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices, GL_STATIC_DRAW);
  
  ShaderProgramSource source = ParseShader("./res/shaders/Basic.glsl");
  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  glUseProgram(shader);


  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  glDeleteProgram(shader);
  glfwTerminate();
  return 0;
}
