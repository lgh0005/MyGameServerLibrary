#pragma once
#include "InstancedRenderBatch.h"
#include "InstanceData.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(QuadMesh)
	MGSL_CLASS_PTR(SpriteRenderBatch)

	class SpriteRenderBatch : public InstancedRenderBatch
	{
		MGSL_DISABLE_COPY(SpriteRenderBatch)
		MGSL_DISABLE_MOVE(SpriteRenderBatch)
		using Super = InstancedRenderBatch;

	public:
		~SpriteRenderBatch() override;
		static SpriteRenderBatchUPtr Create
		(
			const QuadMeshPtr& quadMesh,
			const Texture2DPtr& atlasTexture,
			Shared::uint32 maxInstanceCount
		);

	/*====================================//
	//   default RenderBatch interfaces   //
	//====================================*/
	public:
		virtual void Clear() override;
		virtual void Upload() override;
		virtual void Render() override;

	public:
		bool AddInstance(const SpriteInstanceData& instanceData);
		const Texture2DPtr& GetAtlasTexture() const;

	private:
		SpriteRenderBatch
		(
			const QuadMeshPtr& quadMesh,
			const Texture2DPtr& atlasTexture,
			Shared::uint32 maxInstanceCount
		);

		bool SetInstanceBuffer();
		virtual bool SetInstanceVertexArrayAttrib() override;

		QuadMeshPtr m_quadMesh;
		Texture2DPtr m_atlasTexture;
		Shared::List<SpriteInstanceData> m_instances;
	};
}

