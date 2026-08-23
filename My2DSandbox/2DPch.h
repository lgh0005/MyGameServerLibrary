#pragma once

// Shared
#ifdef _DEBUG
#pragma comment(lib, "MyGameFramework_d.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "ktx_d.lib")
#pragma comment(lib, "freetype.lib")
#else
#pragma comment(lib, "MyGameFramework.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "ktx.lib")
#pragma comment(lib, "freetype.lib")
#endif

#include "MyGameFramework/GameFrameworkPch.h"

// Custom
#include "Enums.h"
#include "Sandbox2DGlobal.h"

// Managers
#include "NetworkManager.h"

#define MGSL_NETWORK_MGR MGSL::Net::NetworkManager::Instance()