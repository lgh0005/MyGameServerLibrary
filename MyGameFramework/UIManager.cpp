#include "GameFrameworkPch.h"
#include "UIManager.h"
#include "Camera.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	UIManager::UIManager() = default;
	UIManager::~UIManager() = default;

	bool UIManager::Init()
	{
		if (!CreateCameraBuffer()) return false;
		return true;
	}

	void UIManager::Update()
	{
		UpdateCameraBuffer();
	}

	void UIManager::Clear()
	{
		ClearUICamera();
	}

	void UIManager::SetReferenceResolution(const Shared::vec2& resolution)
	{
		if (resolution.x <= 0.0f || resolution.y <= 0.0f) return;
		m_referenceResolution = resolution;

		if (!m_uiCamera) return;
		const float aspect = m_referenceResolution.x / m_referenceResolution.y;
		m_uiCamera->SetOrthographic(m_referenceResolution.y, aspect, -100.0f, 100.0f);
	}

	void UIManager::Resize(const Shared::vec2& windowSize)
	{
		if (windowSize.x <= 0.0f || windowSize.y <= 0.0f) return;
		m_windowSize = windowSize;
	}

	const Shared::vec2& UIManager::GetReferenceResolution() const noexcept
	{
		return m_referenceResolution;
	}

	const Shared::vec2& UIManager::GetWindowSize() const noexcept
	{
		return m_windowSize;
	}

	/*=================================//
	//   ui camera setting methods    //
	//=================================*/
	void UIManager::SetUICamera(Camera* camera)
	{
		m_uiCamera = camera;
		if (!m_uiCamera) return;

		const float aspect = m_referenceResolution.x / m_referenceResolution.y;
		m_uiCamera->SetOrthographic(m_referenceResolution.y, aspect, -100.0f, 100.0f);
	}

	Camera* UIManager::GetUICamera() const
	{
		return m_uiCamera;
	}

	void UIManager::ClearUICamera()
	{
		m_uiCamera = nullptr;
	}

	bool UIManager::HasUICamera() const
	{
		return m_uiCamera != nullptr;
	}

	/*===============//
	//   ui camera   //
	//===============*/
	bool UIManager::CreateCameraBuffer()
	{
		m_uiCameraBuffer = GLBuffer::Create
		(
			sizeof(CameraData),
			nullptr,
			GL_DYNAMIC_STORAGE_BIT
		);

		if (!m_uiCameraBuffer) return false;

		BindCameraBuffer();
		return true;
	}

	void UIManager::UpdateCameraBuffer()
	{
		if (!m_uiCamera || !m_uiCameraBuffer) return;

		const CameraData& cameraData = m_uiCamera->GetCameraData();
		m_uiCameraBuffer->SetData
		(
			0,
			sizeof(CameraData),
			&cameraData
		);
	}

	void UIManager::BindCameraBuffer() const
	{
		if (!m_uiCameraBuffer) return;
		m_uiCameraBuffer->BindBase(GL_UNIFORM_BUFFER, (Shared::uint32)EUniformBinding::UI);
	}
}

