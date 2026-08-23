#pragma once

namespace MGSL::Net 
{
	MGSL_CLASS_PTR(ServerService)
	MGSL_CLASS_PTR(GameRoom)
}

namespace MGSL::Server
{
	class Server
	{
		MGSL_DISABLE_COPY(Server)
		MGSL_DISABLE_MOVE(Server)
		static constexpr Shared::int32 IOCP_WORKER_THREAD_COUNT = 5;
		static constexpr Shared::int32 MAX_SESSION_COUNT = 100;

	public:
		Server();
		~Server();

	public:
		bool Init(const Shared::WString& ipAddr, Shared::uint16 port);
		void Update();
		void Clear();

	public:
		Net::GameRoomPtr GetGameRoom() const;

	private:
		Net::ServerServicePtr m_serverService;
		Net::GameRoomPtr m_gameRoom;
	};
}

