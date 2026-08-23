#include "GameFrameworkPch.h"
#include "FlipbookController.h"
#include "FlipbookClip.h"
#include "Texture2D.h"

namespace MGSL::Framework
{
	FlipbookController::FlipbookController(const Texture2DPtr& atlasTexture) : m_atlasTexture(atlasTexture) { }
	FlipbookController::~FlipbookController() = default;

	FlipbookControllerPtr FlipbookController::Create(const Texture2DPtr& atlasTexture)
	{
		MGSL_ASSERT_MSG(atlasTexture, "Atlas texture is null.");

		if (!atlasTexture->IsValid()) return nullptr;
		return FlipbookControllerPtr(new FlipbookController(atlasTexture));
	}

	void FlipbookController::ResizeClips(Shared::usize size)
	{
		m_flipbookClips.resize(size);
	}

	bool FlipbookController::SetClip(Shared::uint32 stateIndex, const FlipbookClipPtr& clip)
	{
		MGSL_ASSERT_MSG(clip, "Clip is null.");
		if (!clip->IsValid()) return false;

		if (stateIndex >= m_flipbookClips.size())
			m_flipbookClips.resize(static_cast<Shared::usize>(stateIndex) + 1);

		m_flipbookClips[stateIndex] = clip;
		return true;
	}

	bool FlipbookController::RemoveClip(Shared::uint32 stateIndex)
	{
		if (stateIndex >= m_flipbookClips.size()) return false;
		if (!m_flipbookClips[stateIndex]) return false;

		m_flipbookClips[stateIndex].reset();
		return true;
	}

	void FlipbookController::Clear()
	{
		m_flipbookClips.clear();
	}

	const Texture2DPtr& FlipbookController::GetAtlasTexture() const
	{
		return m_atlasTexture;
	}

	FlipbookClipPtr FlipbookController::GetClip(Shared::uint32 stateIndex) const
	{
		if (!HasClip(stateIndex)) return nullptr;
		return m_flipbookClips[stateIndex];
	}

	bool FlipbookController::HasClip(Shared::uint32 stateIndex) const
	{
		if (stateIndex >= m_flipbookClips.size()) return false;

		const FlipbookClipPtr& clip = m_flipbookClips[stateIndex];
		if (!clip) return false;

		return clip->IsValid();
	}

	bool FlipbookController::IsValid() const
	{
		return m_atlasTexture && m_atlasTexture->IsValid();
	}
}