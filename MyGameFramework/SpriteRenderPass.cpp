#include "GameFrameworkPch.h"
#include "SpriteRenderPass.h"
#include "SpriteRenderBatch.h"
#include "QuadMesh.h"
#include "Shader.h"

namespace MGSL::Framework
{
	SpriteRenderPass::SpriteRenderPass() = default;
	SpriteRenderPass::~SpriteRenderPass() = default;

	SpriteRenderPassUPtr SpriteRenderPass::Create()
	{
		return SpriteRenderPassUPtr(new SpriteRenderPass());
	}

	bool SpriteRenderPass::Init()
	{
		// 외부에서 셰이더가 먼저 주입되어야 한다.
		if (!m_spriteShader)
		{
			MGSL_LOG_ERROR("SpriteRenderPass : m_spriteShader is null.");
			return false;
		}
		if (!m_spriteShader->IsValid()) return false;

		// Sprite instancing에서 공유할 Quad Mesh 생성
		m_quadMesh = QuadMesh::Create();
		if (!m_quadMesh) return false;

		// Batch Pool 초기화
		if (!m_spriteRenderBatchPool.Init(m_quadMesh, MAX_SPRITE_INSTANCE_COUNT))
			return false;

		return true;
	}

	void SpriteRenderPass::Build()
	{
		for (const auto& renderItem : m_renderQueue.GetRenderItems())
		{
			auto* renderBatch = m_spriteRenderBatchPool.Acquire(renderItem.atlasTexture);
			if (!renderBatch) continue;
			renderBatch->AddInstance(renderItem.instanceData);
		}
	}

	void SpriteRenderPass::Render()
	{
		m_spriteShader->Bind();

		for (const auto& renderBatch : m_spriteRenderBatchPool.GetRenderBatches())
		{
			if (!renderBatch || renderBatch->IsEmpty()) continue;
			renderBatch->Upload();
			renderBatch->Render();
		}

		m_spriteShader->Unbind();
	}

	void SpriteRenderPass::Clear()
	{
		m_renderQueue.Clear();
		m_spriteRenderBatchPool.Clear();
	}

	void SpriteRenderPass::Submit(const SpriteRenderItem& renderItem)
	{
		m_renderQueue.Submit(renderItem);
	}

	void SpriteRenderPass::SetSpriteShader(ShaderPtr spriteShader)
	{
		m_spriteShader = spriteShader;
	}
}