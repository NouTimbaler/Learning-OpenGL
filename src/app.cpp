#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


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

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(640, 480, "My OpenGL Tests App", NULL, NULL);
		if (!window)
		{
				glfwTerminate();
				return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		// Initialitze glew
		if (glewInit() != GLEW_OK){
				std::cout << "error" << std::endl;
				return -1;
		}

		std::cout << glGetString(GL_VERSION) << std::endl;


		float positions[] = {
				-0.5f, -0.5f,
				0.0f, 0.5f,
				0.5f, -0.5f
		};

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		std::string vertexShader =
				"#version 330 core\n"
				"\n"
				"layout(location = 0)in vec4 position;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	gl_Position = position;\n"
				"}\n";
		std::string fragmentShader =
				"#version 330 core\n"
				"\n"
				"layout(location = 0)out vec4 color;\n"
				"\n"
				"void main()\n"
				"{\n"
				"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
				"}\n";
		unsigned int shaders = CreateShader(vertexShader, fragmentShader);
		glUseProgram(shaders);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
				/* Render here */
				glClear(GL_COLOR_BUFFER_BIT);



				glDrawArrays(GL_TRIANGLES, 0, 3);



				/* Swap front and back buffers */
				glfwSwapBuffers(window);

				/* Poll for and process events */
				glfwPollEvents();
		}

		glDeleteProgram(shaders);

		glfwTerminate();
		return 0;
}
