#pragma once

// Shared
#ifdef _DEBUG
#pragma comment(lib, "MyGameShared_d.lib")
#pragma comment(lib, "libprotobufd.lib")
#else
#pragma comment(lib, "MyGameShared.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif

#include "MyGameShared/SharedPch.h"

// Custom
#include "Enums.h"
#include "PacketHeader.h"

// Protobuf
#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Types.pb.h"
#include "Struct.pb.h"
