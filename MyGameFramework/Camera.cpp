#include "GameFrameworkPch.h"
#include "Camera.h"
#include "Transform.h"

namespace MGSL::Framework
{
	Camera::Camera(GameObject* owner) : Super(owner) { }
	Camera::~Camera() = default;

	CameraUPtr Camera::Create(GameObject* owner)
	{
		if (!owner) return nullptr;
		return CameraUPtr(new Camera(owner));
	}

	void Camera::LateUpdate(float deltaTime)
	{
		UpdateViewMatrix();

		if (m_projectionDirty) UpdateProjectionMatrix();
		m_cameraData.view = m_viewMatrix;
		m_cameraData.projection = m_projectionMatrix;
		m_cameraData.viewProjection = m_projectionMatrix * m_viewMatrix;
		m_cameraData.worldPosition = Shared::vec4(GetTransform().GetWorldPosition(), 1.0f);
	}

	void Camera::SetOrthographic
	(
		float viewHeight,
		float aspectRatio,
		float nearPlane,
		float farPlane
	)
	{
		if (viewHeight <= 0.0f) return;
		if (aspectRatio <= 0.0f) return;
		if (nearPlane >= farPlane) return;

		m_orthographicHeight = viewHeight;
		m_aspectRatio = aspectRatio;
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;

		m_projectionDirty = true;
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		if (aspectRatio <= 0.0f) return;
		m_aspectRatio = aspectRatio;
		m_projectionDirty = true;
	}

	/*===============================//
	//   camera matrix getters       //
	//===============================*/
	const CameraData& Camera::GetCameraData() const
	{
		return m_cameraData;
	}

	const Shared::mat4& Camera::GetViewMatrix()
	{
		UpdateViewMatrix();
		return m_viewMatrix;
	}

	const Shared::mat4& Camera::GetProjectionMatrix()
	{
		if (m_projectionDirty) UpdateProjectionMatrix();
		return m_projectionMatrix;
	}

	Shared::mat4 Camera::GetViewProjectionMatrix()
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		const Transform& transform = GetTransform();
		const Shared::vec3 position = transform.GetWorldPosition();
		const Shared::quat rotation = transform.GetWorldQuaternion();
		m_viewMatrix =
			glm::mat4_cast(glm::conjugate(rotation)) *
			glm::translate(Shared::mat4(1.0f), -position);
	}

	void Camera::UpdateProjectionMatrix()
	{
		const float halfHeight = m_orthographicHeight * 0.5f;
		const float halfWidth = halfHeight * m_aspectRatio;
		m_projectionMatrix = glm::ortho
		(
			-halfWidth,
			halfWidth,
			-halfHeight,
			halfHeight,
			m_nearPlane,
			m_farPlane
		);

		m_projectionDirty = false;
	}
}


