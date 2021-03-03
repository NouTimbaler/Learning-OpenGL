#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filename)
		: m_FilePath(filename), m_RendererID(0)
{

		ShaderSources sources = ParseShader(filename);
		GLCALL(m_RendererID = CreateShader(sources.VertexSource, sources.FragmentSource));
}
Shader::~Shader()
{
		GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
		GLCALL(glUseProgram(m_RendererID));
}
void Shader::UnBind() const
{
		GLCALL(glUseProgram(0));
}

//set uniforms
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
		GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
unsigned int Shader::GetUniformLocation(const std::string& name)
{
		if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
				return m_UniformLocationCache[name];

		GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
		if(location == -1)
				std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
		m_UniformLocationCache[name] = location;
		return location;
}


ShaderSources Shader::ParseShader(const std::string& filepath)
{
		std::ifstream stream(filepath);

		enum class ShaderType
		{
				NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
				if (line.find("#shader") != std::string::npos)
				{
						if (line.find("vertex") != std::string::npos)
								type = ShaderType::VERTEX;
						else if (line.find("fragment") != std::string::npos)
								type = ShaderType::FRAGMENT;
				}
				else
				{
						ss[(int)type] << line << '\n';
				}
		}
		return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE)
		{
				int length;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				char* message = (char*)alloca(length * sizeof(char));
				glGetShaderInfoLog(id, length, &length, message);
				std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
				std::cout << message << std::endl;
				return 0;
		}

		return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
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
