#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(FlipbookClip)

	class FlipbookClip
	{
		MGSL_DISABLE_COPY(FlipbookClip)
		MGSL_DISABLE_MOVE(FlipbookClip)

	public:
		~FlipbookClip();
		static FlipbookClipPtr Create
		(
			const Shared::List<Shared::vec4>& frames,
			float framesPerSecond
		);

	public:
		const Shared::vec4& GetUVRect(Shared::uint32 frameIndex) const;
		Shared::uint32 GetFrameCount() const;
		float GetFrameDuration() const;
		bool IsValid() const;

	private:
		FlipbookClip
		(
			const Shared::List<Shared::vec4>& frames,
			float framesPerSecond
		);

		Shared::List<Shared::vec4> m_frames;
		float m_frameDuration = 0.0f;
	};
}



