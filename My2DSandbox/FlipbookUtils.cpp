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

	Framework::FlipbookClipPtr FlipbookUtils::MakeHorizontalClip(Shared::usize frameCount, float fps, bool loop)
	{
		Shared::List<Shared::vec4> frames = MakeHorizontalFrames(frameCount);
		return Framework::FlipbookClip::Create(frames, fps, loop);
	}

	Framework::FlipbookClipPtr FlipbookUtils::MakeAtlasRowClip
	(
		Shared::uint32 row,
		Shared::usize frameCount,
		Shared::uint32 columnCount,
		Shared::uint32 rowCount,
		float fps,
		bool loop
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

		return Framework::FlipbookClip::Create(frames, fps, loop);
	}

	bool FlipbookUtils::AddAtlasRowClip
	(
		const Framework::FlipbookControllerPtr& controller,
		Shared::uint32 stateIndex,
		Shared::uint32 row,
		Shared::usize frameCount,
		Shared::uint32 columnCount,
		Shared::uint32 rowCount,
		float fps,
		bool loop
	)
	{
		if (!controller) return false;
		Framework::FlipbookClipPtr clip = MakeAtlasRowClip(row, frameCount, columnCount, rowCount, fps, loop);
		if (!clip) return false;
		return controller->SetClip(stateIndex, clip);
	}

	/*================================================================//
	//   hardcoded flipbook and flipbook controller loading methods   //
	//================================================================*/
	Framework::FlipbookControllerPtr FlipbookUtils::MakeFighterController(const Framework::Texture2DPtr& texture)
	{
		Framework::FlipbookControllerPtr controller = Framework::FlipbookController::Create(texture);
		if (!controller) return nullptr;

		constexpr Shared::uint32 columnCount = 10;
		constexpr Shared::uint32 rowCount = 12;
		constexpr float fps = 16.0f;

		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_IDLE, 0, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_WALK, 1, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_RUN, 2, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_JUMP, 3, 5, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_FALL, 3, 5, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_DASH, 4, 6, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_CLIMB, 5, 4, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_AIR_ATTACK, 6, 2, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_ATTACK_1, 7, 4, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_ATTACK_2, 8, 7, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_ATTACK_3, 9, 8, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_HIT, 10, 4, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_DEATH, 11, 10, columnCount, rowCount, fps, false)) return nullptr;

		return controller;
	}

	Framework::FlipbookControllerPtr FlipbookUtils::MakePistolController(const Framework::Texture2DPtr& texture)
	{
		Framework::FlipbookControllerPtr controller = Framework::FlipbookController::Create(texture);
		if (!controller) return nullptr;

		constexpr Shared::uint32 columnCount = 10;
		constexpr Shared::uint32 rowCount = 11;
		constexpr float fps = 16.0f;

		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_IDLE, 0, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_WALK, 1, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_RUN, 2, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_JUMP, 3, 5, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_FALL, 3, 5, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_SLIDE, 4, 8, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_DASH, 5, 6, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_CLIMB, 6, 4, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_AIR_ATTACK, 7, 2, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_SHOT, 8, 2, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_HIT, 9, 4, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_DEATH, 10, 10, columnCount, rowCount, fps, false)) return nullptr;

		return controller;
	}

	Framework::FlipbookControllerPtr FlipbookUtils::MakeSwordController(const Framework::Texture2DPtr& texture)
	{
		Framework::FlipbookControllerPtr controller = Framework::FlipbookController::Create(texture);
		if (!controller) return nullptr;

		constexpr Shared::uint32 columnCount = 10;
		constexpr Shared::uint32 rowCount = 13;
		constexpr float fps = 16.0f;

		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_IDLE, 0, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_WALK, 1, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_RUN, 2, 8, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_JUMP, 3, 5, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_FALL, 3, 5, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_SLIDE, 4, 8, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_DASH, 5, 6, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_CLIMB, 6, 4, columnCount, rowCount, fps, true))  return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_AIR_ATTACK, 7, 3, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_ATTACK_1, 8, 4, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_ATTACK_2, 9, 3, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_ATTACK_3, 10, 4, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_HIT, 11, 4, columnCount, rowCount, fps, false)) return nullptr;
		if (!AddAtlasRowClip(controller, Protobuf::OBJECT_STATE_TYPE_DEATH, 12, 10, columnCount, rowCount, fps, false)) return nullptr;

		return controller;
	}
}