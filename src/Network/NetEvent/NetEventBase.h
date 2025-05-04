#pragma once

struct _ENetPeer;
typedef _ENetPeer ENetPeer;

template<typename TNetHostType>
class NetEventBase
{
  public:
	ENetPeer* peer;

	virtual void HandleEvent(TNetHostType& host) = 0;

	virtual ~NetEventBase() {};
};

