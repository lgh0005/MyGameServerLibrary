#include "GameFrameworkPch.h"
#include "FlipbookPlayer.h"
#include "FlipbookClip.h"
#include "FlipbookController.h"
#include "Texture2D.h"
#include "GameObject.h"

namespace MGSL::Framework
{
	FlipbookPlayer::FlipbookPlayer(GameObject* owner, const FlipbookControllerPtr& controller)
		: Super(owner), m_flipbookController(controller) {}
	FlipbookPlayer::~FlipbookPlayer() = default;

	FlipbookPlayerUPtr FlipbookPlayer::Create
	(
		GameObject* owner,
		const FlipbookControllerPtr& controller
	)
	{
		if (!owner || !controller->IsValid()) return nullptr;
		return FlipbookPlayerUPtr(new FlipbookPlayer(owner, controller));
	}

	void FlipbookPlayer::Update(float deltaTime)
	{
		if (!m_isPlaying) return;
		if (!m_flipbookController->IsValid()) return;
		if (!m_currentFlipbookClip->IsValid()) return;

		m_elapsedTime += deltaTime * m_playbackSpeed;
		const float frameDuration = m_currentFlipbookClip->GetFrameDuration();
		while (m_elapsedTime >= frameDuration)
		{
			m_elapsedTime -= frameDuration;
			AdvanceFrame();
			if (!m_isPlaying)
				break;
		}
	}

	void FlipbookPlayer::Submit()
	{
		if (!m_flipbookController->IsValid()) return;

		GameObject* owner = GetOwner();
		if (!owner) return;

		SpriteRenderItem renderItem;
		renderItem.atlasTexture = m_flipbookController->GetAtlasTexture();
		renderItem.instanceData.worldMatrix = owner->GetTransform().GetWorldMatrix();
		renderItem.instanceData.uvRect = m_uvRect;
		renderItem.instanceData.size = m_size;
		renderItem.instanceData.color = m_color;

		MGSL_RENDER_MGR.Submit(renderItem);
	}

	/*==============================//
	//   flipbook state methods     //
	//==============================*/
	bool FlipbookPlayer::SetState(Shared::uint32 stateIndex)
	{
		if (!m_flipbookController->IsValid()) return false;

		const FlipbookClipPtr clip = m_flipbookController->GetClip(stateIndex);
		if (!clip->IsValid()) return false;

		if (m_currentFlipbookClip == clip)
			return true;

		m_currentFlipbookClip = clip;

		m_currentFrame = 0;
		m_elapsedTime = 0.0f;
		m_isPlaying = true;

		ApplyCurrentFrame();

		return true;
	}

	/*=================================//
	//   flipbook playback methods     //
	//=================================*/
	void FlipbookPlayer::Play()  
	{ 
		if (!m_flipbookController->IsValid()) return;
		if (!m_currentFlipbookClip->IsValid()) return;
		m_isPlaying = true; 
	}
	void FlipbookPlayer::Pause() { m_isPlaying = false; }
	void FlipbookPlayer::Stop()
	{
		m_isPlaying = false;
		m_currentFrame = 0;
		m_elapsedTime = 0.0f;
		ApplyCurrentFrame();
	}

	void FlipbookPlayer::SetLoop(bool loop) { m_isLoop = loop; }
	void FlipbookPlayer::SetPlaybackSpeed(float playbackSpeed) { if (playbackSpeed < 0.0f) return; m_playbackSpeed = playbackSpeed; }
	void FlipbookPlayer::SetFrame(Shared::uint32 frameIndex)
	{
		if (!m_currentFlipbookClip->IsValid()) return;
		if (frameIndex >= m_currentFlipbookClip->GetFrameCount()) return;

		m_currentFrame = frameIndex;
		m_elapsedTime = 0.0f;
		ApplyCurrentFrame();
	}

	/*=================================//
	//   sprite rendering methods      //
	//=================================*/
	void FlipbookPlayer::SetSize(const Shared::vec2& size) { m_size = size; }
	void FlipbookPlayer::SetColor(const Shared::vec4& color) { m_color = color; }

	/*=====================//
	//   default getters   //
	//=====================*/
	const FlipbookControllerPtr& FlipbookPlayer::GetController() const { return m_flipbookController; }
	const FlipbookClipPtr& FlipbookPlayer::GetCurrentClip()		 const { return m_currentFlipbookClip; }
	const Shared::vec4& FlipbookPlayer::GetUVRect()						 const { return m_uvRect; }
	const Shared::vec2& FlipbookPlayer::GetSize()						 const { return m_size; }
	const Shared::vec4& FlipbookPlayer::GetColor()						 const { return m_color; }
	Shared::uint32 FlipbookPlayer::GetCurrentFrame()			 const { return m_currentFrame; }
	float FlipbookPlayer::GetPlaybackSpeed()					 const { return m_playbackSpeed; }
	bool FlipbookPlayer::IsPlaying()							 const { return m_isPlaying; }
	bool FlipbookPlayer::IsLoop()								 const { return m_isLoop; }

	void FlipbookPlayer::AdvanceFrame()
	{
		if (!m_currentFlipbookClip->IsValid()) return;

		const Shared::uint32 frameCount = m_currentFlipbookClip->GetFrameCount();
		if (m_currentFrame + 1 < frameCount)
		{
			++m_currentFrame;
			ApplyCurrentFrame();
			return;
		}

		if (m_isLoop)
		{
			m_currentFrame = 0;
			ApplyCurrentFrame();
			return;
		}

		m_currentFrame = frameCount - 1;
		m_isPlaying = false;
		ApplyCurrentFrame();
	}

	void FlipbookPlayer::ApplyCurrentFrame()
	{
		if (!m_currentFlipbookClip->IsValid()) return;

		const Shared::uint32 frameCount = m_currentFlipbookClip->GetFrameCount();
		if (frameCount == 0) return;
		if (m_currentFrame >= frameCount) return;

		m_uvRect = m_currentFlipbookClip->GetUVRect(m_currentFrame);
	}
}


