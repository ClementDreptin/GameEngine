#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {
	
}

void Sandbox2D::OnAttach() {
	EB_PROFILE_FUNCTION();

	m_CheckerboardTexture = Ember::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Ember::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach() {
	EB_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Ember::Timestep ts) {
	EB_PROFILE_SCOPE("CameraController::OnUpdate");

	m_CameraController.OnUpdate(ts);

	Ember::Renderer2D::ResetStats();

	{
		EB_PROFILE_SCOPE("Renderer Prep");
		Ember::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Ember::RenderCommand::Clear();
	}
#if 0
	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		EB_PROFILE_SCOPE("Renderer Draw");
		Ember::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Ember::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Ember::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Ember::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Ember::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, glm::radians(rotation), { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
		Ember::Renderer2D::EndScene();
		
		Ember::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5; y += 0.5f) {
			for (float x = -5.0f; x < 5; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Ember::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Ember::Renderer2D::EndScene();
	}
#endif

	if (Ember::Input::IsMouseButtonPressed(EB_MOUSE_BUTTON_LEFT)) {
		auto [x, y] = Ember::Input::GetMousePosition();
		auto width = Ember::App::Get().GetWindow().GetWidth();
		auto height = Ember::App::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++) {
			m_ParticleSystem.Emit(m_Particle);
		}
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Ember::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Ember::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, { 1.0f, 1.0f }, m_SpriteSheet);
	Ember::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() {
	EB_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Ember::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Ember::Event& e) {
	m_CameraController.OnEvent(e);
}