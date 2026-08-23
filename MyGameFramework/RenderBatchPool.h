#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(RenderBatch)

	class RenderBatchPool
	{
		MGSL_DISABLE_COPY(RenderBatchPool)
			MGSL_DISABLE_MOVE(RenderBatchPool)

	public:
		using BatchUPtr = Shared::UniquePtr<RenderBatch>;
		using BatchList = Shared::List<BatchUPtr>;

	public:
		RenderBatchPool();
		virtual ~RenderBatchPool();

	public:
		void Clear();
		const BatchList& GetRenderBatches() const;

	protected:
		RenderBatch* StoreBatch(BatchUPtr renderBatch);

	private:
		BatchList m_renderBatches;
	};
}