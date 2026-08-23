#include "GameFrameworkPch.h"
#include "RenderPipeline2D.h"
#include "SpriteRenderPass.h"
#include "PostProcessingRenderPass2D.h"
#include "UIRenderPass.h"

// TEMP
#include "DebugRenderPass.h"

namespace MGSL::Framework
{
	RenderPipeline2D::RenderPipeline2D() = default;
	RenderPipeline2D::~RenderPipeline2D() = default;

	RenderPipeline2DUPtr RenderPipeline2D::Create()
	{
		auto renderPipeline2D = RenderPipeline2DUPtr(new RenderPipeline2D());
		auto spriteRenderPass = SpriteRenderPass::Create();
		auto postProcessingRenderPass = PostProcessingRenderPass2D::Create();
		auto uiRenderPass = UIRenderPass::Create();

		// TEMP
		auto debugRenderPass = DebugRenderPass::Create();
		if (!debugRenderPass) return nullptr;
		renderPipeline2D->m_debugRenderPass = std::move(debugRenderPass);

		if (!spriteRenderPass || !postProcessingRenderPass || !uiRenderPass) return nullptr;

		renderPipeline2D->m_spriteRenderPass = std::move(spriteRenderPass);
		renderPipeline2D->m_postProcessingRenderPass2D = std::move(postProcessingRenderPass);
		renderPipeline2D->m_uiRenderPass = std::move(uiRenderPass);
		return renderPipeline2D;
	}

	/*===================================//
	//   default RenderItem submissions  //
	//===================================*/
	void RenderPipeline2D::Submit(const SpriteRenderItem& renderItem)
	{
		if (!m_spriteRenderPass) return;
		m_spriteRenderPass->Submit(renderItem);
	}

	void RenderPipeline2D::Submit(const UIImageRenderItem& renderItem)
	{
		if (!m_uiRenderPass) return;
		m_uiRenderPass->Submit(renderItem);
	}

	void RenderPipeline2D::Submit(const UITextRenderItem& renderItem)
	{
		if (!m_uiRenderPass) return;
		m_uiRenderPass->Submit(renderItem);
	}

	/*============================================//
	//   default RenderPipeline interface impls   //
	//============================================*/
	bool RenderPipeline2D::Init()
	{
		if (!m_spriteRenderPass->Init()) return false;
		if (!m_postProcessingRenderPass2D->Init()) return false;
		if (!m_uiRenderPass->Init()) return false;

		// TEMP
		if (!m_debugRenderPass->Init())
			return false;

		return true;
	}

	void RenderPipeline2D::Build()
	{
		m_spriteRenderPass->Build();
		m_postProcessingRenderPass2D->Build();
		m_uiRenderPass->Build();

		// TEMP
		m_debugRenderPass->Build();
	}

	void RenderPipeline2D::Render()
	{
		// 1. SpriteRenderPass 내용 기록
		m_postProcessingRenderPass2D->BeginRenderTarget();
		m_spriteRenderPass->Render();
		m_postProcessingRenderPass2D->EndRenderTarget();

		// 2. 포스트-프로세싱 효과 추가
		m_postProcessingRenderPass2D->Render();

		// TEMP : Debug Gizmo
		m_debugRenderPass->Render();

		// 3. UI 렌더링
		m_uiRenderPass->Render();
	}

	void RenderPipeline2D::Clear()
	{
		m_spriteRenderPass->Clear();
		m_postProcessingRenderPass2D->Clear();
		m_uiRenderPass->Clear();
	}
}