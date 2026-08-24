#include "GameFrameworkPch.h"
#include "FlipbookClip.h"

namespace MGSL::Framework
{
	FlipbookClip::FlipbookClip(const Shared::List<Shared::vec4>& frames, float framesPerSecond, bool loop)
		: m_frames(frames), m_isLoop(loop)
	{
		if (framesPerSecond > 0.0f)
			m_frameDuration = 1.0f / framesPerSecond;
	}
	FlipbookClip::~FlipbookClip() = default;

	FlipbookClipPtr FlipbookClip::Create(const Shared::List<Shared::vec4>& frames, float framesPerSecond, bool loop)
	{
		if (frames.empty() || framesPerSecond <= 0.0f) return nullptr;
		return FlipbookClipPtr(new FlipbookClip(frames, framesPerSecond, loop));
	}

	const Shared::vec4& FlipbookClip::GetUVRect(Shared::uint32 frameIndex) const
	{
		return m_frames[frameIndex];
	}

	Shared::uint32 FlipbookClip::GetFrameCount() const
	{
		return static_cast<Shared::uint32>(m_frames.size());
	}

	float FlipbookClip::GetFrameDuration() const
	{
		return m_frameDuration;
	}

	bool FlipbookClip::IsValid() const
	{
		return !m_frames.empty() && m_frameDuration > 0.0f;
	}

	void FlipbookClip::SetLoop(bool loop)
	{
		m_isLoop = loop;
	}

	bool FlipbookClip::IsLoop() const 
	{ 
		return m_isLoop; 
	}
}

