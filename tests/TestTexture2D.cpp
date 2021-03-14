#include "TestTexture2D.h"

#include "Errors.h"
#include "Renderer.h"
#include "imgui.h"


namespace test
{

		TestTexture2D::TestTexture2D()
				: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
				m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
				m_Translation(600, 200, 0), m_R(0.0f)
		{
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


				m_VAO = std::make_unique<VertexArray>();
				m_VBO = std::make_unique<VertexBuffer>(positions, 4*4*sizeof(float));
				VertexBufferLayout layout;
				layout.Push<float>(2);
				layout.Push<float>(2);
				m_VAO->AddBuffer(*m_VBO, layout);
				m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);


				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(200, 200, 0));
				model = glm::scale(model, glm::vec3(2, 2, 0));
				glm::mat4 mvp = m_Proj * m_View * model;

				m_Shader = std::make_unique<Shader>("res/shaders/basicTexture.shader");
				m_Shader->Bind();
				m_Shader->SetUniformMat4f("u_MVP", mvp);

				m_Texture = std::make_unique<Texture>("res/textures/halloween.png");
				m_Shader->SetUniform1i("u_Texture", 0);


				m_Shader2 = std::make_unique<Shader>("res/shaders/basic.shader");
				m_Shader2->Bind();
				m_Shader2->SetUniform4f("u_Color", 0.8f, 0.1f, 0.8f, 1.0f);
		}

		TestTexture2D::~TestTexture2D()
		{
		}

		void TestTexture2D::OnUpdate(float deltaTime)
		{
		}

		void TestTexture2D::OnRender()
		{
				Renderer renderer;

				//m_Shader->Bind();
				m_Texture->Bind(0);
				renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
				//shader.UnBind();
				//texture.UnBind();

				glm::mat4 model2 = glm::translate(glm::mat4(1.0f), m_Translation);
				glm::mat4 mvp2 = m_Proj * m_View * model2;
				m_Shader2->Bind();
				m_Shader2->SetUniform4f("u_Color", m_R, 0.1f, 0.8f, 1.0f);
				m_Shader2->SetUniformMat4f("u_MVP", mvp2);
				renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader2);
		}

		void TestTexture2D::OnImGuiRender()
		{
				ImGui::Text("This is the tester window of ImGui");
				ImGui::SliderFloat("Color", &m_R, 0.0f, 1.0f);
				ImGui::SliderFloat3("Position", &m_Translation.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

}
