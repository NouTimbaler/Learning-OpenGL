#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "Errors.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//#define GLCALL(x) x

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
		window = glfwCreateWindow(800, 600, "My OpenGL Tests App", NULL, NULL);
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


		// vertex definition
		float positions[] = {
				-0.5f, -0.5f, 0.0f, 0.0f,
				0.5f, -0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
				0, 1, 2,
				2, 3, 0
		};

		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCALL(glEnable(GL_BLEND));

		VertexArray va;

		VertexBuffer vb(positions, 4*4*sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);


		Shader shader("res/shaders/basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.1f, 0.8f, 1.0f);


		Texture texture("res/textures/halloween.png");
		texture.Bind(0);
		shader.SetUniform1i("u_Texture", 0);

		// unbind everything
		va.UnBind();
		vb.UnBind();
		ib.UnBind();
		shader.UnBind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
				/* Render here */
				renderer.Clear();

				shader.Bind();
				shader.SetUniform4f("u_Color", r, 0.1f, 0.8f, 1.0f);

				renderer.Draw(va, ib, shader);

				r += increment;
				if (r > 1.0f or r < 0.0f) increment *= -1;


				/* Swap front and back buffers */
				glfwSwapBuffers(window);

				/* Poll for and process events */
				glfwPollEvents();
		}

		glfwTerminate();
		return 0;
}
