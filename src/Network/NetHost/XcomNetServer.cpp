#include "XcomNetServer.h"

XcomNetServer::XcomNetServer()
{
	address.host = ENET_HOST_ANY;
	address.port = 7777;
	host = enet_host_create(&address, 2, 2, 0, 0);
}
