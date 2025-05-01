#pragma once
#include "NetHost.h"
#include <boost/signals2.hpp>

enum class EXcomNetEventType : uint8_t
{
	NET_ERROR,

};

class XcomNetHost : public NetHost
{
public:

	virtual ~XcomNetHost() override = default;

protected:

	virtual void HandleReceiveEvent(ENetEvent& event) override;

};
