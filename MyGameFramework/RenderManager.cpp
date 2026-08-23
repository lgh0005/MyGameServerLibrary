#include "GameFrameworkPch.h"
#include "RenderManager.h"
#include "Camera.h"
#include "GLBuffer.h"
#include "RenderPipeline2D.h"
#include "SpriteRenderPass.h"
#include "PostProcessingRenderPass2D.h"
#include "UIRenderPass.h"

// TEMP
#include "DebugRenderPass.h"

namespace MGSL::Framework
{
	RenderManager::RenderManager() = default;
	RenderManager::~RenderManager() = default;

	bool RenderManager::Init()
	{
		// 렌더 파이프라인 구축
		m_renderPipeline2D = RenderPipeline2D::Create();
		if (!m_renderPipeline2D) return false;

		// 카메라 UBO 생성
		if (!CreateCameraBuffer()) return false;

		return true;
	}

	bool RenderManager::Finalize()
	{
		if (!m_renderPipeline2D->Init()) return false;
		return true;
	}

	void RenderManager::Build()
	{
		if (m_renderPipeline2D) m_renderPipeline2D->Build();
	}

	void RenderManager::Render()
	{
		if (!m_mainCamera || !m_cameraBuffer) return;

		// 카메라 버퍼 업데이트
		UpdateCameraBuffer();

		// 파이프라인 렌더링
		if (m_renderPipeline2D) m_renderPipeline2D->Render();
	}

	void RenderManager::Clear()
	{
		if (m_renderPipeline2D) m_renderPipeline2D->Clear();
	}

	/*===============================//
	//   render submission methods   //
	//===============================*/
	void RenderManager::Submit(const SpriteRenderItem& renderItem)
	{
		if (m_renderPipeline2D) m_renderPipeline2D->Submit(renderItem);
	}

	void RenderManager::Submit(const UIImageRenderItem& renderItem)
	{
		if (m_renderPipeline2D) m_renderPipeline2D->Submit(renderItem);
	}

	void RenderManager::Submit(const UITextRenderItem& renderItem)
	{
		if (m_renderPipeline2D) m_renderPipeline2D->Submit(renderItem);
	}

	/*=================================//
	//   main camera setting methods   //
	//=================================*/
	void RenderManager::SetMainCamera(Camera* camera) { m_mainCamera = camera; }
	Camera* RenderManager::GetMainCamera() const { return m_mainCamera; }
	void RenderManager::ClearMainCamera() { m_mainCamera = nullptr; }
	bool RenderManager::HasMainCamera() const { return m_mainCamera != nullptr; }

	/*====================//
	//   shader setters   //
	//====================*/
	void RenderManager::SetSpriteShader(ShaderPtr spriteShader)
	{
		m_renderPipeline2D->GetSpriteRenderPass()->SetSpriteShader(spriteShader);
	}

	void RenderManager::SetPostProcessing2DShader(ShaderPtr postProcessingShader)
	{
		m_renderPipeline2D->GetPostProcessingRenderPass2D()->SetPostProcessingShader(postProcessingShader);
	}

	void RenderManager::SetUIImageShader(ShaderPtr postProcessingShader)
	{
		m_renderPipeline2D->GetUIRenderPass()->SetUIImageShader(postProcessingShader);
	}

	void RenderManager::SetUITextShader(ShaderPtr postProcessingShader)
	{
		m_renderPipeline2D->GetUIRenderPass()->SetUITextShader(postProcessingShader);
	}

	void RenderManager::SetDebugShader(ShaderPtr shader)
	{
		m_renderPipeline2D->GetDebugRenderPass()->SetDebugShader(shader);
	}

	/*=================//
	//   main camera   //
	//=================*/
	bool RenderManager::CreateCameraBuffer()
	{
		m_cameraBuffer = GLBuffer::Create(sizeof(CameraData), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (!m_cameraBuffer) return false;

		BindCameraBuffer();
		return true;
	}

	void RenderManager::UpdateCameraBuffer()
	{
		const CameraData& cameraData = m_mainCamera->GetCameraData();
		m_cameraBuffer->SetData(0, sizeof(CameraData), &cameraData);
	}

	void RenderManager::BindCameraBuffer() const
	{
		if (!m_cameraBuffer) return;
		m_cameraBuffer->BindBase(GL_UNIFORM_BUFFER, (Shared::uint32)EUniformBinding::CAMERA);
	}
}

