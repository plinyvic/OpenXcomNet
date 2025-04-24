#pragma once

#include "NetEventBase.h"

class NetEventDisconnect : public NetEventBase
{
public:

	virtual void HandleEvent(NetHost& host) override {};

};

