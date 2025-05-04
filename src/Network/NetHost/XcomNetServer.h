#pragma once
#include "../../Network/NetHost/XcomNetHost.h"

#include "../../enet/enet.h"

class XcomNetServer : public XcomNetHost
{
public:

	XcomNetServer();

private:

	ENetAddress address;



};
