#pragma once
#include "MyGameFramework/MonoBehaviour.h"

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(CameraController)

	class CameraController : public Framework::MonoBehaviour
	{
		MGSL_DISABLE_COPY(CameraController)
		MGSL_DISABLE_MOVE(CameraController)
		using Super = Framework::MonoBehaviour;

	public:
		virtual ~CameraController() override;
		static CameraControllerUPtr Create(Framework::GameObject* owner);

	public:
		void LateUpdate(float deltaTime) override;
		void SetTarget(Framework::GameObject* target);

	private:
		explicit CameraController(Framework::GameObject* owner);

		Framework::GameObject* m_target = nullptr;
	};
}

