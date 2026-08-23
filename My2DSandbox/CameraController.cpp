#include "2DPch.h"
#include "CameraController.h"
#include "MyGameFramework/KeyboardDevice.h"

namespace MGSL::Sandbox2D
{
	CameraController::CameraController(Framework::GameObject* owner) : Super(owner) { }
	CameraController::~CameraController() = default;

	CameraControllerUPtr CameraController::Create(Framework::GameObject* owner)
	{
		return CameraControllerUPtr(new CameraController(owner));
	}

	void CameraController::LateUpdate(float deltaTime)
	{
		if (!m_target) return;

		const Shared::vec3 targetPosition = m_target->GetTransform().GetPosition();
		Shared::vec3 cameraPosition = GetTransform().GetPosition();
		
		cameraPosition.x = targetPosition.x;
		GetTransform().SetPosition(cameraPosition);
	}

	void CameraController::SetTarget(Framework::GameObject* target) { m_target = target; }
}