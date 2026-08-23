#pragma once

// Shared
#ifdef _DEBUG
#pragma comment(lib, "MyGameServerProtocol_d.lib")
#else
#pragma comment(lib, "MyGameServerProtocol.lib")
#endif

#include "MyGameServerProtocol/ServerProtoPch.h"

// winsock
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

// Custom
#include "Enums.h"
#include "SocketUtils.h"
#include "NetAddress.h"