#pragma once

#include <enet/enet.h>
#include <memory>
#include <queue>
#include "../../Network/NetworkController/OutboundPacket.h"

template<typename T>
struct PacketData;

template<typename T>
class TNetEventReceive;

class NetHost
{
public:
	ENetHost* host;

	virtual void Application() = 0;

	~NetHost();

	

	void SetPacketBuffer(const std::shared_ptr<std::queue<OutboundPacket>>& controllerBuffer)
	{
		outboundPacketBuffer = controllerBuffer;
	}

protected:

	std::shared_ptr<std::queue<OutboundPacket>> outboundPacketBuffer;

};
