#pragma once

#include <enet/enet.h>

class NetHost;


class NetEventBase
{
public:

	ENetPeer* peer;

	virtual void HandleEvent(NetHost& host) = 0;

	virtual ~NetEventBase() {};
};

