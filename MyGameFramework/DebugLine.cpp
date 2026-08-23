#include "GameFrameworkPch.h"
#include "DebugLine.h"

namespace MGSL::Framework
{
	DebugLine::DebugLine() = default;
	DebugLine::DebugLine(const Shared::vec3& start, const Shared::vec3& end, const Shared::vec4& color)
		: m_start(start), m_end(end), m_color(color) { }
	DebugLine::~DebugLine() = default;
}