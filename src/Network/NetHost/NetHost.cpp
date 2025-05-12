#include "NetHost.h"

#include "../../enet/enet.h"

#ifdef _DEBUG
#include <limits>
#endif

NetHost::~NetHost()
{
	if (host != nullptr)
	{
		enet_host_destroy(host);
	}
}

void NetHost::HandleENetEvents()
{
	ENetEvent event;
	while (enet_host_service(host, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
			ConnectEvent connectEvent{event.peer};
			HandleConnectEvent(connectEvent);
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
			HandleDisconnectEvent(event);
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			HandleReceiveEvent(event);
			break;
		}
	}
}

void NetHost::HandleConnectEvent(ConnectEvent& event)
{
	#ifdef _DEBUG
	// disable timeout if a debug build. debugger breaks will cause timeout.
	enet_peer_timeout(event.peer, std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());
	// ok, doesnt disable it, but makes it really far into the future.
	#endif
	connectEventSignal(event);
}

void NetHost::HandleDisconnectEvent(ENetEvent& event)
{
	disconnectEventSignal(event);
}

void NetHost::HandleReceiveEvent(ENetEvent& event)
{

}
