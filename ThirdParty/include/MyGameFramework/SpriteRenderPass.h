#pragma once
#include "RenderPass.h"
#include "RenderQueue.h"
#include "SpriteRenderBatchPool.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Shader)
	MGSL_CLASS_PTR(SpriteRenderPass)

	class SpriteRenderPass : public RenderPass
	{
		MGSL_DISABLE_COPY(SpriteRenderPass)
		MGSL_DISABLE_MOVE(SpriteRenderPass)
		using Super = RenderPass;
		using SpriteRenderQueue = RenderQueue<SpriteRenderItem>;
		static constexpr Shared::uint32 MAX_SPRITE_INSTANCE_COUNT = 10000;

	public:
		virtual ~SpriteRenderPass() override;
		static SpriteRenderPassUPtr Create();
		void Submit(const SpriteRenderItem& renderItem);

	public:
		bool Init() override;
		void Build() override;
		void Render() override;
		void Clear() override;

	private:
		SpriteRenderPass();

		SpriteRenderQueue m_renderQueue;
		SpriteRenderBatchPool m_spriteRenderBatchPool;
		QuadMeshPtr m_quadMesh;

	/*=====================================//
	//   SpriteRenderPass shader members   //
	//=====================================*/
	public:
		void SetSpriteShader(ShaderPtr shader);

	private:
		ShaderPtr m_spriteShader;
	};
}

