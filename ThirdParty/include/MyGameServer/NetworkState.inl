#pragma once

namespace MGSL::Server
{
	template<typename TInfo>
	NetworkState<TInfo>::NetworkState(GameObject* owner) : Super(owner) {}

	template<typename TInfo>
	NetworkState<TInfo>::~NetworkState() = default;

	template<typename TInfo>
	void NetworkState<TInfo>::SetInfo(const TInfo& info)
	{
		m_info = info;
	}

	template<typename TInfo>
	void NetworkState<TInfo>::SetInfo(TInfo&& info)
	{
		m_info = std::move(info);
	}

	template<typename TInfo>
	TInfo& NetworkState<TInfo>::GetInfo() noexcept
	{
		return m_info;
	}

	template<typename TInfo>
	const TInfo& NetworkState<TInfo>::GetInfo() const noexcept
	{
		return m_info;
	}
}