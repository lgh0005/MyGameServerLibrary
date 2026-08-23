#pragma once
#include "MyGameShared/Types.h"

namespace MGSL::Framework
{
	struct Glyph
	{
		Shared::vec2 uvMin = Shared::vec2(0.0f);
		Shared::vec2 uvMax = Shared::vec2(0.0f);
		Shared::ivec2 size = Shared::ivec2(0);
		Shared::ivec2 bearing = Shared::ivec2(0);
		Shared::uint32 advance = 0;
	};
}

