#include "GameFrameworkPch.h"
#include "DebugRenderPass.h"
#include "BoxCollider.h"
#include "Shader.h"

namespace MGSL::Framework
{
	DebugRenderPass::DebugRenderPass() = default;

	DebugRenderPass::~DebugRenderPass()
	{
		Clear();
	}

	DebugRenderPassUPtr DebugRenderPass::Create()
	{
		return DebugRenderPassUPtr(new DebugRenderPass());
	}

	bool DebugRenderPass::Init()
	{
		m_debugRenderer.Init();
		m_vertices.reserve(512);

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		if (m_vao == 0 || m_vbo == 0) return false;

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), reinterpret_cast<void*>(offsetof(DebugVertex, position)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), reinterpret_cast<void*>(offsetof(DebugVertex, color)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return true;
	}

	void DebugRenderPass::Build()
	{
		m_debugRenderer.Clear();
		m_vertices.clear();

		for (BoxCollider* collider : MGSL_COLLIDE_MGR.GetColliders())
		{
			if (!collider) continue;
			const DebugGizmo gizmo = collider->GetDebugGizmo();
			if (collider->IsTrigger()) m_debugRenderer.DrawBox(gizmo, Shared::vec4(1.0f, 0.0f, 1.0f, 1.0f));
			else m_debugRenderer.DrawBox(gizmo, Shared::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		}

		const auto& lines = m_debugRenderer.GetLines();
		m_vertices.reserve(std::max(m_vertices.capacity(), lines.size() * 2));
		for (const DebugLine& line : lines)
		{
			m_vertices.push_back(DebugVertex{ line.GetStart(), line.GetColor() });
			m_vertices.push_back(DebugVertex{ line.GetEnd(), line.GetColor() });
		}
	}

	void DebugRenderPass::Render()
	{
		if (m_vertices.empty())
			return;

		m_debugShader->Bind();
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_vertices.size() * sizeof(DebugVertex)), m_vertices.data(), GL_DYNAMIC_DRAW);
		const GLboolean depthEnabled = glIsEnabled(GL_DEPTH_TEST);
		::glDisable(GL_DEPTH_TEST);
		::glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
		if (depthEnabled) glEnable(GL_DEPTH_TEST);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		m_debugShader->Unbind();
	}

	void DebugRenderPass::Clear()
	{
		m_debugRenderer.Clear();
		m_vertices.clear();

		if (m_vbo != 0)
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}

		if (m_vao != 0)
		{
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
	}

	void DebugRenderPass::SetDebugShader(ShaderPtr shader)
	{
		m_debugShader = shader;
	}
}


