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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Test.h"
#include "TestClearColor.h"
#include "TestTexture2D.h"

//#define GLCALL(x) x

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
		glViewport(0,0,width,height);
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
		std::cout << glGetString(GL_VERSION) << std::endl;

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCALL(glEnable(GL_BLEND));

		Renderer renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
		ImGui::StyleColorsDark();


		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture");


		while (!glfwWindowShouldClose(window))
		{
				renderer.Clear();

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (currentTest)
				{
						currentTest->OnUpdate(0.0f);
						currentTest->OnRender();
						ImGui::Begin("Test");
						if (currentTest != testMenu && ImGui::Button("<-"))
						{
								delete currentTest;
								currentTest = testMenu;

						}
						currentTest->OnImGuiRender();
						ImGui::End();
				}

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(window);
				glfwPollEvents();
		}

		delete currentTest;
		if (currentTest != testMenu)
				delete testMenu;

		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
		return 0;
}
