#pragma once

#include "RendererAPI.h"

namespace Ember {
	class RenderCommand {
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void SetViewport(uint32_t originX, uint32_t originY, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(originX, originY, width, height);
		}
		
		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static Ref<RendererAPI> s_RendererAPI;
	};
}