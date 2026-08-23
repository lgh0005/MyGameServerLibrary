#pragma once

namespace MGSL::Shared
{
	/*=============================//
	//   Threading & Concurrency   //
	//=============================*/
	template<typename T> using Atomic = std::atomic<T>;
	using Thread = std::thread;
	using Mutex = std::mutex;
	using CondVar = std::condition_variable;
	using UniqueLock = std::unique_lock<std::mutex>;
	using LockGuard = std::lock_guard<std::mutex>;

	/*===================//
	//   Smart Pointers  //
	//===================*/
	template<typename T> using UniquePtr = std::unique_ptr<T>;
	template<typename T> using SharedPtr = std::shared_ptr<T>;
	template<typename T> using WeakPtr = std::weak_ptr<T>;
	template<typename T> using Self = std::enable_shared_from_this<T>;

	/*===============================//
	//   C# Style Collections Alias  //
	//===============================*/
	template<typename T, usize N> using Array = std::array<T, N>;
	template<typename T> using List = std::vector<T>;
	template<typename T> using Queue = std::queue<T>;
	template<typename T> using Stack = std::stack<T>;
	template<typename T> using HashSet = std::unordered_set<T>;
	template<typename K, typename V> using Dictionary = std::unordered_map<K, V>;
	using String = std::string;
	using WString = std::wstring;

	/*====================================//
	//   C# Style Functional & Delegates  //
	//====================================*/
	template<typename... Args> using Action = std::function<void(Args...)>;
	template<typename R, typename... Args> using Func = std::function<R(Args...)>;

	/*=======================//
	//   C# Style Nullable   //
	//=======================*/
	template<typename T> using Nullable = std::optional<T>;

	/*===================//
	//   Miscellaneous   //
	//===================*/
	template<typename T> using SourceType = std::remove_reference_t<T>;
	template<typename T, typename U> using Pair = std::pair<T, U>;
}
