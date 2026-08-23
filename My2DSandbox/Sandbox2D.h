#pragma once
#include "MyGameFramework/Game.h"

namespace MGSL::Sandbox2D
{
	class Sandbox2D : public Framework::Game
	{
		using Super = Framework::Game;

	public:
		Sandbox2D();
		virtual ~Sandbox2D() override;

	public:
		bool Init(const Framework::WindowDesc& desc);
		void Run();
		void Clear();

	protected:
		virtual void OnSceneReady() override;
	};
}

