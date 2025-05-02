#pragma once

#include "../../enet/enet.h"

template<typename TNetHostType>
class NetEventBase
{
  public:
	ENetPeer* peer;

	virtual void HandleEvent(TNetHostType& host) = 0;

	virtual ~NetEventBase() {};
};

