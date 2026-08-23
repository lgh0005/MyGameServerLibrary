#pragma once
#include "RenderPass.h"
#include "DebugRenderer.h"
#include "DebugVertex.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Shader)
	MGSL_CLASS_PTR(DebugRenderPass)

	class DebugRenderPass : public RenderPass
	{
		using Super = RenderPass;

	public:
		virtual ~DebugRenderPass() override;
		static DebugRenderPassUPtr Create();

	public:
		bool Init() override;
		void Build() override;
		void Render() override;
		void Clear() override;

	/*====================================//
	//   DebugRenderPass shader members   //
	//====================================*/
	public:
		void SetDebugShader(ShaderPtr shader);

	private:
		DebugRenderPass();

	private:
		DebugRenderer m_debugRenderer;
		Shared::List<DebugVertex> m_vertices;

		GLuint m_vao = 0;
		GLuint m_vbo = 0;

		ShaderPtr m_debugShader;
	};
}

