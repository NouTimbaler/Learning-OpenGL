#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderSources
{
		std::string VertexSource;
		std::string FragmentSource;
};

class Shader
{
private:
		std::string m_FilePath;
		unsigned int m_RendererID;

		std::unordered_map<std::string, int> m_UniformLocationCache;

public:
		Shader(const std::string& filename);
		~Shader();

		void Bind() const;
		void UnBind() const;

		//set uniforms
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform1i(const std::string& name, int value);
private:
		int GetUniformLocation(const std::string& name);
		ShaderSources ParseShader(const std::string& filepath);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int CompileShader(const std::string& source, unsigned int type);
};
