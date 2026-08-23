#include "GameFrameworkPch.h"
#include "UIRenderPass.h"
#include "UIImageRenderBatch.h"
#include "UITextRenderBatch.h"
#include "QuadMesh.h"
#include "Shader.h"

namespace MGSL::Framework
{
	UIRenderPass::UIRenderPass() = default;
	UIRenderPass::~UIRenderPass() = default;

	UIRenderPassUPtr UIRenderPass::Create()
	{
		return UIRenderPassUPtr(new UIRenderPass());
	}

	/*===============================//
	//   render submission methods   //
	//===============================*/
	void UIRenderPass::Submit(const UIImageRenderItem& renderItem)
	{
		m_imageRenderQueue.Submit(renderItem);
	}

	void UIRenderPass::Submit(const UITextRenderItem& renderItem)
	{
		m_textRenderQueue.Submit(renderItem);
	}

	/*============================================//
	//   default RenderPass interface impls       //
	//============================================*/
	bool UIRenderPass::Init()
	{
		// 셰이더 확인
		if (!m_imageShader)
		{
			MGSL_LOG_ERROR("UIRenderPass : m_imageShader is null.");
			return false;
		}

		if (!m_textShader)
		{
			MGSL_LOG_ERROR("UIRenderPass : m_textShader is null.");
			return false;
		}

		if (!m_imageShader->IsValid()) return false;
		if (!m_textShader->IsValid()) return false;

		// UI 메시 생성 및 초기화
		m_quadMesh = QuadMesh::Create();
		if (!m_quadMesh) return false;

		if (!m_imageRenderBatchPool.Init(m_quadMesh, MAX_UI_IMAGE_INSTANCE_COUNT)) return false;
		if (!m_textRenderBatchPool.Init(m_quadMesh, MAX_UI_TEXT_INSTANCE_COUNT)) return false;

		return true;
	}

	void UIRenderPass::Build()
	{
		BuildUIImageRenderItems();
		BuildUITextRenderItems();
	}

	void UIRenderPass::Render()
	{
		RenderUIImageRenderBatches();
		RenderUITextRenderBatches();
	}

	void UIRenderPass::Clear()
	{
		m_imageRenderQueue.Clear();
		m_textRenderQueue.Clear();

		m_imageRenderBatchPool.Clear();
		m_textRenderBatchPool.Clear();
	}

	/*=========================================//
	//   default build and render logics       //
	//=========================================*/
	void UIRenderPass::BuildUIImageRenderItems()
	{
		for (const auto& renderItem : m_imageRenderQueue.GetRenderItems())
		{
			auto* renderBatch = m_imageRenderBatchPool.Acquire(renderItem.texture);
			if (!renderBatch) continue;
			renderBatch->AddInstance(renderItem.instanceData);
		}
	}

	void UIRenderPass::BuildUITextRenderItems()
	{
		for (const auto& renderItem : m_textRenderQueue.GetRenderItems())
		{
			auto* renderBatch = m_textRenderBatchPool.Acquire(renderItem.font);
			if (!renderBatch) continue;
			renderBatch->AddInstances(renderItem.instances);
		}
	}

	void UIRenderPass::RenderUIImageRenderBatches()
	{
		if (!m_imageShader) return;

		m_imageShader->Bind();
		for (const auto& renderBatch : m_imageRenderBatchPool.GetRenderBatches())
		{
			if (!renderBatch || renderBatch->IsEmpty()) continue;
			renderBatch->Upload();
			renderBatch->Render();
		}
		m_imageShader->Unbind();
	}

	void UIRenderPass::RenderUITextRenderBatches()
	{
		if (!m_textShader) return;

		m_textShader->Bind();
		for (const auto& renderBatch : m_textRenderBatchPool.GetRenderBatches())
		{
			if (!renderBatch || renderBatch->IsEmpty()) continue;
			renderBatch->Upload();
			renderBatch->Render();
		}
		m_textShader->Unbind();
	}

	/*==============================//
	//   UIRenderPass shader setters //
	//==============================*/
	void UIRenderPass::SetUIImageShader(ShaderPtr uiImageShader)
	{
		m_imageShader = uiImageShader;
	}

	void UIRenderPass::SetUITextShader(ShaderPtr uiTextShader)
	{
		m_textShader = uiTextShader;
	}
}