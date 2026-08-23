#pragma once

namespace MGSL::Framework
{
	class DebugLine
	{
	public:
		DebugLine();
		DebugLine(const Shared::vec3& start, const Shared::vec3& end, const Shared::vec4& color);
		~DebugLine();

	public:
		const Shared::vec3& GetStart() const { return m_start; }
		const Shared::vec3& GetEnd() const { return m_end; }
		const Shared::vec4& GetColor() const { return m_color; }

	private:
		Shared::vec3 m_start{ 0.0f };
		Shared::vec3 m_end{ 0.0f };
		Shared::vec4 m_color{ 1.0f };
	};
}