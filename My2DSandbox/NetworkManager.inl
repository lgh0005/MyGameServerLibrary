#pragma once

namespace MGSL::Net
{
	template<typename T>
	inline bool NetworkManager::RunOnMainThread(T&& func)
	{
		return PushMainThreadTask(MainThreadTask(std::forward<T>(func)));
	}
}