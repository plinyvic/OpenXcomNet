#pragma once
#include "../../Network/NetHost/XcomNetHost.h"

class XcomNetClient : public XcomNetHost
{
public:

	XcomNetClient(std::string ipAddress);

	~XcomNetClient() override;

private:

	ENetAddress address;
	ENetPeer* server;
};
