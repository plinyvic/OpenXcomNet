#include "NetHost.h"

NetHost::~NetHost()
{
	if (host != nullptr)
	{
		enet_host_destroy(host);
	}
}
