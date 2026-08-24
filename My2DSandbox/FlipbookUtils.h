#pragma once

namespace MGSL::Framework 
{ 
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(FlipbookClip) 
	MGSL_CLASS_PTR(FlipbookController)
}

namespace MGSL::Sandbox2D
{
	/*=======================================//
	//   temp filpbook util static methods   //
	//=======================================*/
	class FlipbookUtils
	{
		MGSL_DISABLE_COPY(FlipbookUtils)
		MGSL_DISABLE_MOVE(FlipbookUtils)

	public:
		static Shared::List<Shared::vec4> MakeHorizontalFrames(Shared::usize frameCount);
		static Framework::FlipbookClipPtr MakeHorizontalClip(Shared::usize frameCount, float fps, bool loop);
		static Framework::FlipbookClipPtr MakeAtlasRowClip
		(
			Shared::uint32 row,
			Shared::usize frameCount,
			Shared::uint32 columnCount,
			Shared::uint32 rowCount,
			float fps,
			bool loop
		);

		static bool AddAtlasRowClip
		(
			const Framework::FlipbookControllerPtr& controller,
			Shared::uint32 stateIndex,
			Shared::uint32 row,
			Shared::usize frameCount,
			Shared::uint32 columnCount,
			Shared::uint32 rowCount,
			float fps,
			bool loop
		);

	/*================================================================//
	//   hardcoded flipbook and flipbook controller loading methods   //
	//================================================================*/
	public:
		static Framework::FlipbookControllerPtr MakeFighterController(const Framework::Texture2DPtr& texture);
		static Framework::FlipbookControllerPtr MakePistolController(const Framework::Texture2DPtr& texture);
		static Framework::FlipbookControllerPtr MakeSwordController(const Framework::Texture2DPtr& texture);
	};
}

