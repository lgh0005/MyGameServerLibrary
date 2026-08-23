#pragma once
#include "Service.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(Listener)
	MGSL_CLASS_PTR(ServerService)

	class ServerService : public Service
	{
		using Super = Service;

	public:
		ServerService
		(
			NetAddress targetAddress,
			NetIOCPCorePtr core,
			SessionFactory factory,
			Shared::int32 maxSessionCount = 1
		);
		virtual ~ServerService() override;

	public:
		virtual bool	Start() override;
		void			CloseService();

	private:
		ListenerPtr m_listener;
	};
}

