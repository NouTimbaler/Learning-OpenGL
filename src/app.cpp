#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

//#define GLCALL(x) x
struct ShaderSources
{
		std::string VertexSource;
		std::string FragmentSource;
};

static ShaderSources ParseShader(const std::string& filepath)
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

static unsigned int CompileShader(const std::string& source, unsigned int type)
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
		GLFWwindow* window;

		/* Initialize glfw */
		if (!glfwInit())
				return -1;


		// Core version 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(600, 600, "My OpenGL Tests App", NULL, NULL);
		if (!window)
		{
				glfwTerminate();
				return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);


		// VSYNC
		glfwSwapInterval(1);

		// Initialitze glew
		if (glewInit() != GLEW_OK){
				std::cout << "error" << std::endl;
				return -1;
		}

		std::cout << glGetString(GL_VERSION) << std::endl;


		{
		// vertex definition
		float positions[] = {
				-0.5f, -0.5f,
				0.5f, -0.5f,
				0.5f, 0.5f,
				-0.5f, 0.5f
		};

		unsigned int indices[] = {
				0, 1, 2,
				2, 3, 0
		};


		unsigned int vao;
		GLCALL(glGenVertexArrays(1, &vao));
		GLCALL(glBindVertexArray(vao));

		VertexArray va;
		VertexBuffer vb(positions, 4*2*sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);


		// Shaders creation and linking
		ShaderSources sources = ParseShader("res/shaders/basic.shader");
		unsigned int shaders = CreateShader(sources.VertexSource, sources.FragmentSource);
		GLCALL(glUseProgram(shaders));

		// Send uniforms
		GLCALL(int location = glGetUniformLocation(shaders, "u_Color"));
		GLCALL(glUniform4f(location, 0.8f, 0.1f, 0.8f, 1.0f));


		// unbind everything
		GLCALL(glBindVertexArray(0));
		GLCALL(glUseProgram(0));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
				/* Render here */
				glClear(GL_COLOR_BUFFER_BIT);


				GLCALL(glUseProgram(shaders));
				GLCALL(glUniform4f(location, r, 0.1f, 0.8f, 1.0f));

				va.Bind();
				ib.Bind();

				//glDrawArrays(GL_TRIANGLES, 0, 3);
				GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

				r += increment;
				if (r > 1.0f or r < 0.0f) increment *= -1;


				/* Swap front and back buffers */
				glfwSwapBuffers(window);

				/* Poll for and process events */
				glfwPollEvents();
		}

		glDeleteProgram(shaders);
		}
		glfwTerminate();
		return 0;
}
