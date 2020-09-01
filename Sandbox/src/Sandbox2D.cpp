#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {
	
}

void Sandbox2D::OnAttach() {
	EB_PROFILE_FUNCTION();

	m_CheckerboardTexture = Ember::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {
	EB_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Ember::Timestep ts) {
	EB_PROFILE_SCOPE("CameraController::OnUpdate");

	m_CameraController.OnUpdate(ts);

	{
		EB_PROFILE_SCOPE("Renderer Prep");
		Ember::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Ember::RenderCommand::Clear();
	}

	{
		EB_PROFILE_SCOPE("Renderer Draw");
		Ember::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Ember::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Ember::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 45, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Ember::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 45, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Ember::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	EB_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Ember::Event& e) {
	m_CameraController.OnEvent(e);
}