#pragma once
#include "Component.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(FlipbookClip)
	MGSL_CLASS_PTR(FlipbookController)
	MGSL_CLASS_PTR(FlipbookPlayer)

	class FlipbookPlayer : public Component
	{
		MGSL_DISABLE_COPY(FlipbookPlayer)
		MGSL_DISABLE_MOVE(FlipbookPlayer)
		using Super = Component;

	public:
		~FlipbookPlayer();
		static FlipbookPlayerUPtr Create
		(
			GameObject* owner,
			const FlipbookControllerPtr& controller
		);

		virtual void Update(float deltaTime) override;
		virtual void Submit() override;

	/*==============================//
	//   flipbook state methods     //
	//==============================*/
	public:
		bool SetState(Shared::uint32 stateIndex);

	/*=================================//
	//   flipbook playback methods     //
	//=================================*/
	public:
		void Play();
		void Pause();
		void Stop();

		void SetLoop(bool loop);
		void SetPlaybackSpeed(float playbackSpeed);
		void SetFrame(Shared::uint32 frameIndex);

	/*=================================//
	//   sprite rendering methods      //
	//=================================*/
	public:
		void SetSize(const Shared::vec2& size);
		void SetColor(const Shared::vec4& color);

	/*=====================//
	//   default getters   //
	//=====================*/
	public:
		const FlipbookControllerPtr& GetController() const;
		const FlipbookClipPtr& GetCurrentClip() const;

		const Shared::vec4& GetUVRect() const;
		const Shared::vec2& GetSize() const;
		const Shared::vec4& GetColor() const;

		Shared::uint32 GetCurrentFrame() const;
		float GetPlaybackSpeed() const;
		bool IsPlaying() const;
		bool IsLoop() const;

	private:
		FlipbookPlayer
		(
			GameObject* owner,
			const FlipbookControllerPtr& controller
		);
		void AdvanceFrame();
		void ApplyCurrentFrame();

		FlipbookControllerPtr m_flipbookController;
		FlipbookClipPtr m_currentFlipbookClip;

		Shared::uint32 m_currentFrame = 0;
		Shared::vec4 m_uvRect{ 0.0f, 0.0f, 1.0f, 1.0f };
		Shared::vec2 m_size{ 1.0f };
		Shared::vec4 m_color{ 1.0f };

		float m_elapsedTime = 0.0f;
		float m_playbackSpeed = 1.0f;

		bool m_isPlaying = false;
		bool m_isLoop = true;
	};
}



