#pragma once
#include "DebugGizmo.h"
#include "DebugLine.h"

namespace MGSL::Framework
{
	class DebugRenderer
	{
	public:
		DebugRenderer();
		~DebugRenderer();

	public:
		void Init();
		void Clear();

	public:
		void DrawLine(const Shared::vec3& start, const Shared::vec3& end, const Shared::vec4& color);
		void DrawBox(const DebugGizmo& bounds, const Shared::vec4& color);
		const Shared::List<DebugLine>& GetLines() const;

	private:
		Shared::List<DebugLine> m_lines;
	};
}

