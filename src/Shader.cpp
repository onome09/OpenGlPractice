#include "Shader.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Renderer.hpp"

Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0)
{
  ShaderProgramSource source = ParseShader(filepath);
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);   
}

Shader::~Shader()
{
  GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
  GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
  GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
  int location = GetUniformLocation(name);
  GLCall(glUniform4f(location, v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std :: string & name)
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()){
    return m_UniformLocationCache[name];
  }
  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

  if (location == -1){
    std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
  }
  m_UniformLocationCache[name] = location;
  return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath){
  std::ifstream stream(filepath);

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


// strings are the source code of the vertex shader and fragment shader
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
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

unsigned int Shader::CompileShader(const std::string& source, unsigned int type){
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
