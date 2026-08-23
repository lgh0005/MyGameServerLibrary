#include "ServerLibPch.h"
#include "PacketSession.h"
#include "MyGameServerProtocol/PacketHeader.h"

namespace MGSL::Net
{
	PacketSession::PacketSession() = default;
	PacketSession::~PacketSession() = default;

	// [size(2)][id(2)][data....][size(2)][id(2)][data....]...
	Shared::int32 PacketSession::OnRecv(BYTE* buffer, Shared::int32 len)
	{
		// 현재까지 진행한 길이
		Shared::int32 processLen = 0;

		while (true)
		{
			// 처리를 해서 이제 남은 데이터 길이는 얼마인지 계산
			Shared::int32 dataSize = len - processLen;

			// 최소한 헤더는 파싱이 가능해야 함
			if (dataSize < sizeof(Protocol::PacketHeader))
				break;

			// 받은 데이터의 헤더 얻기
			Protocol::PacketHeader header = *(reinterpret_cast<Protocol::PacketHeader*>(&buffer[processLen]));

			// 헤더에 기록된 패킷 크기를 파싱할 수 있어야 한다.
			if (dataSize < header.size)
				break;

			// 패킷 조립 성공
			OnRecvPacket(&buffer[processLen], header.size);

			processLen += header.size;
		}

		return processLen;
	}
}
