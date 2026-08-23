#pragma once
#include "Component.h"
#include "Uniform.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Camera)

	class Camera : public Component
	{
		MGSL_DISABLE_COPY(Camera)
		MGSL_DISABLE_MOVE(Camera)
		using Super = Component;

	public:
		virtual ~Camera() override;
		static CameraUPtr Create(GameObject* owner);
		virtual void LateUpdate(float deltaTime) override;

	public:
		void SetOrthographic(float verticalFov, float aspectRatio, float nearPlane, float farPlane);
		void SetAspectRatio(float aspectRatio);

	/*===============================//
	//   camera matrix getters       //
	//===============================*/
	public:
		const CameraData& GetCameraData() const;
		const Shared::mat4& GetViewMatrix();
		const Shared::mat4& GetProjectionMatrix();
		Shared::mat4 GetViewProjectionMatrix();

	private:
		Camera(GameObject* owner);
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();

	private:
		CameraData m_cameraData;

		// Orthographic 전용 설정
		float m_orthographicHeight = 10.0f;

		// 공통 Projection 설정
		float m_aspectRatio = 16.0f / 9.0f;
		float m_nearPlane = 0.1f;
		float m_farPlane = 1000.0f;

		Shared::mat4 m_viewMatrix = Shared::mat4(1.0f);
		Shared::mat4 m_projectionMatrix = Shared::mat4(1.0f);

		bool m_projectionDirty = true;
	};
}



