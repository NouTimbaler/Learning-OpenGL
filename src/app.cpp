#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
		window = glfwCreateWindow(960, 540, "My OpenGL Tests App", NULL, NULL);
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


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
		ImGui::StyleColorsDark();

		std::cout << glGetString(GL_VERSION) << std::endl;


		// vertex definition
		float positions[] = {
				0.0f, 0.0f, 0.0f, 0.0f,
				100.0f, 0.0f, 1.0f, 0.0f,
				100.0f, 100.0f, 1.0f, 1.0f,
				0.0f, 100.0f, 0.0f, 1.0f
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

		va.UnBind();
		vb.UnBind();
		ib.UnBind();

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		glm::mat4 mvp = proj * view * model;

		Shader shader("res/shaders/basic.shader");
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/halloween.png");
		texture.Bind(0);
		shader.SetUniform1i("u_Texture", 0);

		texture.UnBind();
		shader.UnBind();


		Shader shader2("res/shaders/basic2.shader");
		shader2.Bind();
		shader2.SetUniform4f("u_Color", 0.8f, 0.1f, 0.8f, 1.0f);


		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		glm::vec3 translation(600, 200, 0);
		float f = 0.0f;
		char buf[200];

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
				/* Render here */
				renderer.Clear();


				shader.Bind();
				texture.Bind();
				renderer.Draw(va, ib, shader);
				shader.UnBind();
				texture.UnBind();

		glm::mat4 model2 = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mvp2 = proj * view * model2;
				shader2.Bind();
				shader2.SetUniform4f("u_Color", f, 0.1f, 0.8f, 1.0f);
		shader2.SetUniformMat4f("u_MVP", mvp2);
				renderer.Draw(va, ib, shader2);
				shader2.UnBind();

				r += increment;
				if (r > 1.0f or r < 0.0f) increment *= -1;



				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("Tester");
				ImGui::Text("This is the tester window of ImGui");
				ImGui::SliderFloat("Color", &f, 0.0f, 1.0f);
				ImGui::SliderFloat3("Position", &translation.x, 0.0f, 960.0f);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



				/* Swap front and back buffers */
				glfwSwapBuffers(window);

				/* Poll for and process events */
				glfwPollEvents();
		}

		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
		return 0;
}
