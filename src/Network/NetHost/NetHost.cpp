#include "NetHost.h"

#include "../../enet/enet.h"

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
			ConnectEvent connectEvent;
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
	connectEventSignal(event);
}

void NetHost::HandleDisconnectEvent(ENetEvent& event)
{
	disconnectEventSignal(event);
}

void NetHost::HandleReceiveEvent(ENetEvent& event)
{

}
