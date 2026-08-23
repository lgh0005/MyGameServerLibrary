#pragma once
#include "InstancedRenderBatch.h"
#include "InstanceData.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(QuadMesh)
	MGSL_CLASS_PTR(UIImageRenderBatch)

	class UIImageRenderBatch : public InstancedRenderBatch
	{
		MGSL_DISABLE_COPY(UIImageRenderBatch)
		MGSL_DISABLE_MOVE(UIImageRenderBatch)
		using Super = InstancedRenderBatch;

	public:
		~UIImageRenderBatch() override;
		static UIImageRenderBatchUPtr Create
		(
			const QuadMeshPtr& quadMesh,
			const Texture2DPtr& texture,
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
		bool AddInstance(const UIInstanceData& instanceData);
		const Texture2DPtr& GetTexture() const;

	private:
		UIImageRenderBatch
		(
			const QuadMeshPtr& quadMesh,
			const Texture2DPtr& texture,
			Shared::uint32 maxInstanceCount
		);

		bool SetInstanceBuffer();
		virtual bool SetInstanceVertexArrayAttrib() override;

		QuadMeshPtr m_quadMesh;
		Texture2DPtr m_texture;
		Shared::List<UIInstanceData> m_instances;
	};
}

