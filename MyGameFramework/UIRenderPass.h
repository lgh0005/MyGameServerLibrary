#pragma once
#include "RenderPass.h"
#include "RenderQueue.h"
#include "UIImageRenderBatchPool.h"
#include "UITextRenderBatchPool.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Shader)
	MGSL_CLASS_PTR(UIRenderPass)

	class UIRenderPass : public RenderPass
	{
		MGSL_DISABLE_COPY(UIRenderPass)
		MGSL_DISABLE_MOVE(UIRenderPass)
		using Super = RenderPass;
		using UIImageRenderQueue = RenderQueue<UIImageRenderItem>;
		using UITextRenderQueue = RenderQueue<UITextRenderItem>;
		static constexpr Shared::uint32 MAX_UI_IMAGE_INSTANCE_COUNT = 10000;
		static constexpr Shared::uint32 MAX_UI_TEXT_INSTANCE_COUNT = 10000;

	public:
		virtual ~UIRenderPass() override;
		static UIRenderPassUPtr Create();
		void Submit(const UIImageRenderItem& renderItem);
		void Submit(const UITextRenderItem& renderItem);

	public:
		bool Init() override;
		void Build() override;
		void Render() override;
		void Clear() override;

	public:
		void SetUIImageShader(ShaderPtr uiImageShader);
		void SetUITextShader(ShaderPtr uiTextShader);

	private:
		void BuildUIImageRenderItems();
		void BuildUITextRenderItems();
		void RenderUIImageRenderBatches();
		void RenderUITextRenderBatches();

	private:
		UIRenderPass();
		QuadMeshPtr m_quadMesh;

		ShaderPtr m_imageShader;
		UIImageRenderQueue m_imageRenderQueue;
		UIImageRenderBatchPool m_imageRenderBatchPool;

		ShaderPtr m_textShader;
		UITextRenderQueue m_textRenderQueue;
		UITextRenderBatchPool m_textRenderBatchPool;
	};
}

