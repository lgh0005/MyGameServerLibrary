#pragma once

namespace MGSL::Framework
{
	class RenderBatch
	{
		MGSL_DISABLE_COPY(RenderBatch)
		MGSL_DISABLE_MOVE(RenderBatch)

	public:
		virtual ~RenderBatch();

	/*====================================//
	//   default RenderBatch interfaces   //
	//====================================*/
	public:
		virtual void Clear()  = 0;
		virtual void Upload() = 0;
		virtual void Render() = 0;

	public:
		bool IsEmpty() const;
		bool IsFull() const;
		Shared::uint32 GetItemCount() const;
		Shared::uint32 GetMaxItemCount() const;

	protected:
		RenderBatch(Shared::uint32 maxItemCount);

		bool CanAddItem() const;
		void IncreaseItemCount();
		void ResetItemCount();

	private:
		Shared::uint32 m_itemCount = 0;
		Shared::uint32 m_maxItemCount = 0;
	};
}

