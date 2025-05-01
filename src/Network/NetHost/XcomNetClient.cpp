#include "XcomNetClient.h"
#include "../../enet/enet.h"

XcomNetClient::XcomNetClient(std::string ipAddress)
{
	enet_address_set_host(&address, ipAddress.c_str());
	address.port = 7777;
	host = enet_host_create(nullptr, 1, 2, 0, 0);
	server = enet_host_connect(host, &address, 2, 0);
}

XcomNetClient::~XcomNetClient()
{
	// this will leak for now
	//delete server;
}
