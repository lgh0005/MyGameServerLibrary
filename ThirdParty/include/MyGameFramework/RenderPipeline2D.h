#pragma once
#include "MyGameFramework/RenderPipeline.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(RenderPipeline2D)
	MGSL_CLASS_PTR(SpriteRenderPass)
	MGSL_CLASS_PTR(PostProcessingRenderPass2D)
	MGSL_CLASS_PTR(UIRenderPass)

	// TEMP
	MGSL_CLASS_PTR(DebugRenderPass)

	class RenderPipeline2D : public RenderPipeline
	{
		MGSL_DISABLE_COPY(RenderPipeline2D)
		MGSL_DISABLE_MOVE(RenderPipeline2D)
		using Super = RenderPipeline;

	public:
		static RenderPipeline2DUPtr Create();
		virtual ~RenderPipeline2D() override;

	/*===================================//
	//   default RenderItem submissions  //
	//===================================*/
	public:
		void Submit(const SpriteRenderItem& renderItem);
		void Submit(const UIImageRenderItem& renderItem);
		void Submit(const UITextRenderItem& renderItem);

	/*============================================//
	//   default RenderPipeline interface impls   //
	//============================================*/
	public:
		virtual bool Init() override;
		virtual void Build() override;
		virtual void Render() override;
		virtual void Clear() override;

	/*============================================//
	//   defualt RenderPipeline2D render-passes   //
	//============================================*/
	public:
		SpriteRenderPass* GetSpriteRenderPass() { return m_spriteRenderPass.get(); }
		PostProcessingRenderPass2D* GetPostProcessingRenderPass2D() { return m_postProcessingRenderPass2D.get(); }
		UIRenderPass* GetUIRenderPass() { return m_uiRenderPass.get(); }

		// TEMP
		DebugRenderPass* GetDebugRenderPass() { return m_debugRenderPass.get(); }

	private:
		RenderPipeline2D();

		SpriteRenderPassUPtr m_spriteRenderPass;
		PostProcessingRenderPass2DUPtr m_postProcessingRenderPass2D;
		UIRenderPassUPtr m_uiRenderPass;

		// TEMP
		DebugRenderPassUPtr m_debugRenderPass;
	};
}
