#include "GameFrameworkPch.h"
#include "UITextRenderBatchPool.h"
#include "UITextRenderBatch.h"
#include "Font.h"

namespace MGSL::Framework
{
	UITextRenderBatchPool::UITextRenderBatchPool() = default;
	UITextRenderBatchPool::~UITextRenderBatchPool() = default;

	bool UITextRenderBatchPool::Init
	(
		const QuadMeshPtr& quadMesh,
		Shared::uint32 maxInstanceCount
	)
	{
		if (!quadMesh || maxInstanceCount == 0) return false;
		m_quadMesh = quadMesh;
		m_maxInstanceCount = maxInstanceCount;

		return true;
	}

	UITextRenderBatch* UITextRenderBatchPool::Acquire(const FontPtr& font)
	{
		if (!m_quadMesh || !font)
			return nullptr;

		UITextRenderBatch* available = GetAvailableBatch(font);
		if (available) return available;

		auto renderBatch = UITextRenderBatch::Create(m_quadMesh, font, m_maxInstanceCount);
		if (!renderBatch) return nullptr;

		UITextRenderBatch* storedBatch = renderBatch.get();
		StoreBatch(std::move(renderBatch));

		return storedBatch;
	}

	UITextRenderBatch* UITextRenderBatchPool::GetAvailableBatch(const FontPtr& font)
	{
		for (const BatchUPtr& renderBatch : GetRenderBatches())
		{
			if (!renderBatch || renderBatch->IsFull()) continue;

			UITextRenderBatch* textRenderBatch = static_cast<UITextRenderBatch*>(renderBatch.get());
			if (textRenderBatch->GetFont() != font) continue;

			return textRenderBatch;
		}

		return nullptr;
	}
}
