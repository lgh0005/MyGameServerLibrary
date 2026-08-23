#include "GameFrameworkPch.h"
#include "DebugRenderer.h"

namespace MGSL::Framework
{
	DebugRenderer::DebugRenderer() = default;
	DebugRenderer::~DebugRenderer() = default;

	void DebugRenderer::Init() { m_lines.reserve(256); }
	void DebugRenderer::Clear() { m_lines.clear(); }

	void DebugRenderer::DrawLine(const Shared::vec3& start, const Shared::vec3& end, const Shared::vec4& color)
	{
		m_lines.emplace_back(start, end, color);
	}

	void DebugRenderer::DrawBox(const DebugGizmo& bounds, const Shared::vec4& color)
	{
		const Shared::vec3 leftBottom { bounds.min.x, bounds.min.y, 0.0f };
		const Shared::vec3 leftTop { bounds.min.x, bounds.max.y, 0.0f };
		const Shared::vec3 rightTop { bounds.max.x, bounds.max.y, 0.0f };
		const Shared::vec3 rightBottom { bounds.max.x, bounds.min.y, 0.0f };

		DrawLine(leftBottom, leftTop, color);
		DrawLine(leftTop, rightTop, color);
		DrawLine(rightTop, rightBottom, color);
		DrawLine(rightBottom, leftBottom, color);
	}

	const Shared::List<DebugLine>& DebugRenderer::GetLines() const
	{
		return m_lines;
	}
}


