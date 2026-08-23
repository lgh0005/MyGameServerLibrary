#pragma once
#include "MyGameFramework/Scene.h"

namespace MGSL::Framework { MGSL_CLASS_PTR(FlipbookController) }

namespace MGSL::Sandbox2D
{
	class Sandbox2DScene : public Framework::Scene
	{
	public:
		Sandbox2DScene();
		virtual ~Sandbox2DScene() override;

	public:
		virtual bool Load()		  override;
		virtual void Build()	  override;
	};
}

