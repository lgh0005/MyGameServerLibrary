#pragma once
#include "Service.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(ClientService)

	class ClientService : public Service
	{
		using Super = Service;

	public:
		ClientService
		(
			NetAddress targetAddress,
			NetIOCPCorePtr core,
			SessionFactory factory,
			Shared::int32 maxSessionCount = 1
		);
		virtual ~ClientService() override;
		
	public:
		virtual bool Start() override;
	};
}


