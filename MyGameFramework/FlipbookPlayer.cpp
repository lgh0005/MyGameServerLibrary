#include "GameFrameworkPch.h"
#include "FlipbookPlayer.h"
#include "FlipbookClip.h"
#include "FlipbookController.h"
#include "GameObject.h"

namespace MGSL::Framework
{
	FlipbookPlayer::FlipbookPlayer(GameObject* owner) : Super(owner) { }
	FlipbookPlayer::~FlipbookPlayer() = default;

	FlipbookPlayerUPtr FlipbookPlayer::Create(GameObject* owner)
	{
		return FlipbookPlayerUPtr(new FlipbookPlayer(owner));
	}

	void FlipbookPlayer::Update(float deltaTime)
	{
		if (!m_isPlaying) return;
		if (!m_currentFlipbookController) return;
		if (!m_currentFlipbookController->IsValid()) return;
		if (!m_currentFlipbookClip) return;
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
		if (!m_currentFlipbookController) return;
		if (!m_currentFlipbookController->IsValid()) return;
		if (!m_currentFlipbookClip) return;
		if (!m_currentFlipbookClip->IsValid()) return;

		GameObject* owner = GetOwner();
		if (!owner) return;

		SpriteRenderItem renderItem;
		renderItem.atlasTexture = m_currentFlipbookController->GetAtlasTexture();
		renderItem.instanceData.worldMatrix = owner->GetTransform().GetWorldMatrix();
		renderItem.instanceData.uvRect = m_uvRect;
		renderItem.instanceData.size = m_size;
		renderItem.instanceData.color = m_color;

		MGSL_RENDER_MGR.Submit(renderItem);
	}
	/*==================================//
	//   flipbook controller methods    //
	//==================================*/
	bool FlipbookPlayer::SetController(Shared::uint32 controllerIndex, const FlipbookControllerPtr& controller)
	{
		if (!controller) return false;
		if (!controller->IsValid()) return false;
		if (controllerIndex >= m_flipbookControllers.size())
			m_flipbookControllers.resize(static_cast<Shared::usize>(controllerIndex) + 1);

		m_flipbookControllers[controllerIndex] = controller;
		return true;
	}

	bool FlipbookPlayer::RemoveController(Shared::uint32 controllerIndex)
	{
		if (controllerIndex >= m_flipbookControllers.size()) return false;
		FlipbookControllerPtr& controller = m_flipbookControllers[controllerIndex];
		if (!controller) return false;

		if (m_currentFlipbookController == controller)
		{
			m_currentFlipbookController.reset();
			m_currentFlipbookClip.reset();
			m_currentFrame = 0;
			m_elapsedTime = 0.0f;
			m_isPlaying = false;
		}

		controller.reset();
		return true;
	}

	bool FlipbookPlayer::ChangeController(Shared::uint32 controllerIndex)
	{
		if (!HasController(controllerIndex)) return false;
		FlipbookControllerPtr controller = m_flipbookControllers[controllerIndex];
		if (m_currentFlipbookController == controller)
			return true;

		m_currentFlipbookController = controller;
		m_currentFlipbookClip.reset();
		m_currentFrame = 0;
		m_elapsedTime = 0.0f;
		m_isPlaying = false;

		return true;
	}

	void FlipbookPlayer::ResizeControllers(Shared::usize size)
	{
		m_flipbookControllers.resize(size);
	}

	void FlipbookPlayer::ClearControllers()
	{
		m_flipbookControllers.clear();
		m_currentFlipbookController.reset();
		m_currentFlipbookClip.reset();
		m_currentFrame = 0;
		m_elapsedTime = 0.0f;
		m_isPlaying = false;
	}

	/*==============================//
	//   flipbook state methods     //
	//==============================*/
	bool FlipbookPlayer::SetState(Shared::uint32 stateIndex)
	{
		if (!m_currentFlipbookController) return false;
		if (!m_currentFlipbookController->IsValid()) return false;

		FlipbookClipPtr clip = m_currentFlipbookController->GetClip(stateIndex);
		if (!clip) return false;
		if (!clip->IsValid()) return false;
		if (m_currentFlipbookClip == clip) return true;

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
		if (!m_currentFlipbookController) return;
		if (!m_currentFlipbookController->IsValid()) return;
		if (!m_currentFlipbookClip) return;
		if (!m_currentFlipbookClip->IsValid()) return;
		m_isPlaying = true;
	}

	void FlipbookPlayer::Pause() 
	{
		m_isPlaying = false; 
	}

	void FlipbookPlayer::Stop()
	{
		m_isPlaying = false;
		m_currentFrame = 0;
		m_elapsedTime = 0.0f;
		ApplyCurrentFrame();
	}

	void FlipbookPlayer::SetPlaybackSpeed(float playbackSpeed) 
	{ 
		if (playbackSpeed < 0.0f) return; 
		m_playbackSpeed = playbackSpeed; 
	}

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
	void FlipbookPlayer::SetFlipX(bool flipX)
	{
		if (m_flipX == flipX) return;
		m_flipX = flipX;
		ApplyCurrentFrame();
	}

	/*=====================//
	//   default getters   //
	//=====================*/
	FlipbookControllerPtr FlipbookPlayer::GetController(Shared::uint32 controllerIndex) const 
	{ 
		if (!HasController(controllerIndex)) return nullptr; 
		return m_flipbookControllers[controllerIndex];
	}

	const FlipbookClipPtr& FlipbookPlayer::GetCurrentClip()		 const { return m_currentFlipbookClip; }
	const Shared::vec4& FlipbookPlayer::GetUVRect()				 const { return m_uvRect; }
	const Shared::vec2& FlipbookPlayer::GetSize()				 const { return m_size; }
	const Shared::vec4& FlipbookPlayer::GetColor()				 const { return m_color; }
	bool FlipbookPlayer::IsFlipX()								 const{ return m_flipX; }
	Shared::uint32 FlipbookPlayer::GetCurrentFrame()			 const { return m_currentFrame; }
	float FlipbookPlayer::GetPlaybackSpeed()					 const { return m_playbackSpeed; }

	bool FlipbookPlayer::HasController(Shared::uint32 controllerIndex) const
	{
		if (controllerIndex >= m_flipbookControllers.size()) return false;
		const FlipbookControllerPtr& controller = m_flipbookControllers[controllerIndex];
		if (!controller) return false;
		return controller->IsValid();
	}

	bool FlipbookPlayer::IsPlaying()							 const { return m_isPlaying; }

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

		if (m_currentFlipbookClip->IsLoop())
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
		if (m_flipX)
		{
			m_uvRect.x += m_uvRect.z;
			m_uvRect.z *= -1.0f;
		}
	}
}


