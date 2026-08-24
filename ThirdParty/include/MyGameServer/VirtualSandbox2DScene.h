#pragma once
#include "VirtualScene.h"

namespace MGSL::Server
{
	class VirtualSandbox2DScene : public VirtualScene
	{
		MGSL_DISABLE_COPY(VirtualSandbox2DScene)
		MGSL_DISABLE_MOVE(VirtualSandbox2DScene)
		using Super = VirtualScene;

	public:
		VirtualSandbox2DScene();
		virtual ~VirtualSandbox2DScene() override;

	private:
		virtual void Build() override;
	};
}

