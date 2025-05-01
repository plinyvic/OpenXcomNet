#pragma once
#include "../../Network/NetHost/XcomNetHost.h"

class XcomNetServer : public XcomNetHost
{
public:

	XcomNetServer();

private:

	ENetAddress address;



};
