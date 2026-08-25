#pragma once

namespace MGSL::Server
{
	template<typename TInfo>
	class INetworkState
	{
	public:
		virtual ~INetworkState() = default;
		virtual Shared::uint64 GetObjectID() const    = 0;
		virtual void SetObjectID(Shared::uint64 objectID) = 0;
		virtual void SetInfo(const TInfo& info)	      = 0;
		virtual void SetInfo(TInfo&& info)		      = 0;
		virtual TInfo& GetInfo() noexcept		      = 0;
		virtual const TInfo& GetInfo() const noexcept = 0;
	};
}
