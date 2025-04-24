#pragma once

#include "NetEventBase.h"

class NetEventConnect : public NetEventBase
{
public:

	virtual void HandleEvent(NetHost& host) override {};

};