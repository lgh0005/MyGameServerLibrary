#pragma once
#include "InstancedRenderBatch.h"
#include "InstanceData.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Font)
	MGSL_CLASS_PTR(QuadMesh)
	MGSL_CLASS_PTR(UITextRenderBatch)

	class UITextRenderBatch : public InstancedRenderBatch
	{
		MGSL_DISABLE_COPY(UITextRenderBatch)
		MGSL_DISABLE_MOVE(UITextRenderBatch)
		using Super = InstancedRenderBatch;

	public:
		~UITextRenderBatch() override;
		static UITextRenderBatchUPtr Create
		(
			const QuadMeshPtr& quadMesh,
			const FontPtr& font,
			Shared::uint32 maxInstanceCount
		);

	public:
		void Clear() override;
		void Upload() override;
		void Render() override;

	public:
		bool AddInstance(const UIInstanceData& instanceData);
		bool AddInstances(const Shared::List<UIInstanceData>& instances);

		const FontPtr& GetFont() const;

	private:
		UITextRenderBatch
		(
			const QuadMeshPtr& quadMesh,
			const FontPtr& font,
			Shared::uint32 maxInstanceCount
		);

		bool SetInstanceBuffer();
		bool SetInstanceVertexArrayAttrib() override;

	private:
		QuadMeshPtr m_quadMesh;
		FontPtr m_font;

		Shared::List<UIInstanceData> m_instances;
	};
}

