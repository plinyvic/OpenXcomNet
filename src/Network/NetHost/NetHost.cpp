#include "NetHost.h"
#include <exception>

NetHost::~NetHost()
{
	if (host != nullptr)
	{
		enet_host_destroy(host);
	}
}

void NetHost::SetPacketBuffer(const std::shared_ptr<std::queue<OutboundPacket> >& controllerBuffer)
{
	outboundPacketBuffer = controllerBuffer;
}

void NetHost::HandleENetEvents()
{
	ENetEvent event;
	while (enet_host_service(host, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			HandleConnectEvent(event);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			HandleDisconnectEvent(event);
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			HandleReceiveEvent(event);
			break;
		}
	}
}

void NetHost::HandleConnectEvent(ENetEvent& event)
{
	connectEventSignal(event);
}

void NetHost::HandleDisconnectEvent(ENetEvent& event)
{
	disconnectEventSignal(event);
}
