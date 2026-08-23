#pragma once

// Shared
#ifdef _DEBUG
#pragma comment(lib, "MyGameServerLibrary_d.lib")
#pragma comment(lib, "MyGameServerProtocol_d.lib")
#pragma comment(lib, "libprotobufd.lib")
#else
#pragma comment(lib, "MyGameServerLibrary.lib")
#pragma comment(lib, "MyGameServerProtocol.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif

#include "MyGameServerProtocol/ServerProtoPch.h"
#include "MyGameServerLibrary/ServerLibPch.h"

// Customs
#include "Enums.h"
#include "ServerGlobal.h"

// Managers
#include "GameSessionManager.h"
#include "ObjectManager.h"
#include "ServerTimeManager.h"
#include "ServerCollisionManager.h"

#define MGSL_SESSION_MGR MGSL::Net::GameSessionManager::Instance()
#define MGSL_OBJECT_MGR MGSL::Server::ObjectManager::Instance()
#define MGSL_SERVER_TIME_MGR MGSL::Server::ServerTimeManager::Instance()
#define MGSL_SERVER_COLLISION_MGR MGSL::Server::ServerCollisionManager::Instance()