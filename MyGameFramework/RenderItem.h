#pragma once
#include "InstanceData.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(Font)

	struct SpriteRenderItem
	{
		Texture2DPtr atlasTexture;
		SpriteInstanceData instanceData;
	};

	struct UIImageRenderItem
	{
		Texture2DPtr texture;
		UIInstanceData instanceData;
	};

	struct UITextRenderItem
	{
		FontPtr font;
		Shared::List<UIInstanceData> instances;
	};
}