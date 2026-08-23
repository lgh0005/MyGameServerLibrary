#pragma once

namespace MGSL::Net
{

	template<typename T>
	static inline bool SetSockOpt
	(
		SOCKET socket, 
		Shared::int32 level, 
		Shared::int32 optName,
		T optVal
	)
	{
		return ::setsockopt
		(
			socket, 
			level, 
			optName, 
			reinterpret_cast<char*>(&optVal), 
			sizeof(T)
		) != SOCKET_ERROR;
	}
}