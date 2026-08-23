#include "2DPch.h"
#include "FlipbookUtils.h"
#include "MyGameFramework/FlipbookClip.h"
#include "MyGameFramework/FlipbookController.h"

namespace MGSL::Sandbox2D
{
	Shared::List<Shared::vec4> FlipbookUtils::MakeHorizontalFrames(Shared::usize frameCount)
	{
		Shared::List<Shared::vec4> frames;
		frames.reserve(frameCount);

		const float frameWidth = 1.0f / static_cast<float>(frameCount);
		for (Shared::usize i = 0; i < frameCount; ++i)
			frames.emplace_back(static_cast<float>(i) * frameWidth, 0.0f, frameWidth, 1.0f);

		return frames;
	}

	Framework::FlipbookClipPtr FlipbookUtils::MakeHorizontalClip(Shared::usize frameCount, float fps)
	{
		Shared::List<Shared::vec4> frames = MakeHorizontalFrames(frameCount);
		return Framework::FlipbookClip::Create(frames, fps);
	}

	Framework::FlipbookClipPtr FlipbookUtils::MakeAtlasRowClip
	(
		Shared::uint32 row,
		Shared::usize frameCount,
		Shared::uint32 columnCount,
		Shared::uint32 rowCount,
		float fps
	)
	{
		if (columnCount == 0) return nullptr;
		if (rowCount == 0) return nullptr;
		if (frameCount == 0) return nullptr;
		if (frameCount > columnCount) return nullptr;
		if (row >= rowCount) return nullptr;

		const float frameWidth = 1.0f / static_cast<float>(columnCount);
		const float frameHeight = 1.0f / static_cast<float>(rowCount);
		const float frameY = static_cast<float>(row) * frameHeight;

		Shared::List<Shared::vec4> frames;
		frames.reserve(frameCount);

		for (Shared::usize frame = 0; frame < frameCount; ++frame)
			frames.emplace_back(static_cast<float>(frame) * frameWidth, frameY, frameWidth, frameHeight);

		return Framework::FlipbookClip::Create(frames, fps);
	}

	bool FlipbookUtils::AddAtlasRowClip
	(
		const Framework::FlipbookControllerPtr& controller,
		Shared::uint32 stateIndex,
		Shared::uint32 row,
		Shared::usize frameCount,
		Shared::uint32 columnCount,
		Shared::uint32 rowCount,
		float fps
	)
	{
		if (!controller) return false;
		Framework::FlipbookClipPtr clip = MakeAtlasRowClip(row, frameCount, columnCount, rowCount, fps);
		if (!clip) return false;
		return controller->SetClip(stateIndex, clip);
	}
}