#pragma once

namespace MGSL::Framework 
{ 
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
		static Framework::FlipbookClipPtr MakeHorizontalClip(Shared::usize frameCount, float fps);
		static Framework::FlipbookClipPtr MakeAtlasRowClip
		(
			Shared::uint32 row,
			Shared::usize frameCount,
			Shared::uint32 columnCount,
			Shared::uint32 rowCount,
			float fps
		);

		static bool AddAtlasRowClip
		(
			const Framework::FlipbookControllerPtr& controller,
			Shared::uint32 stateIndex,
			Shared::uint32 row,
			Shared::usize frameCount,
			Shared::uint32 columnCount,
			Shared::uint32 rowCount,
			float fps
		);
	};
}

