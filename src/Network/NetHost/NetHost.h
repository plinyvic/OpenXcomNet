#pragma once

#include "../../enet/enet.h"
#include <memory>
#include <queue>
#include <string>
#include "../../Network/NetworkController/OutboundPacket.h"
#include <boost/signals2.hpp>
#include <map>

// Constraints requires a callable type (functor), the return type, and any argument types.
template <typename TFunction, typename TReturn, typename... TArgs>
concept CallableWithSignature = std::invocable<TFunction, TArgs...> &&
								std::is_convertible_v<std::invoke_result_t<TFunction, TArgs...>, TReturn>;

struct ConnectEvent
{
	// reserved
};

class NetHost
{
public:

	ENetHost* host;

protected:

	ENetAddress address;

private:

	boost::signals2::signal<void(ConnectEvent&)> connectEventSignal;
	boost::signals2::signal<void(ENetEvent&)> disconnectEventSignal;

public:

	virtual ~NetHost()
	{
		if (host != nullptr)
		{
			enet_host_destroy(host);
		}
	}

	void HandleENetEvents()
	{
		ENetEvent event;
		while (enet_host_service(host, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				ConnectEvent connectEvent;
				HandleConnectEvent(connectEvent);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
				HandleDisconnectEvent(event);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				HandleReceiveEvent(event);
				break;
			}
		}
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, ConnectEvent&>
	inline boost::signals2::connection BindToOnConnectEvent(TCallable&& function)
	{
		return connectEventSignal.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, ENetEvent&>
	inline boost::signals2::connection BindToOnDisconnectEvent(TCallable&& function)
	{
		return disconnectEventSignal.connect(std::forward<TCallable>(function));
	}

protected:

	virtual void HandleConnectEvent(ConnectEvent& event)
	{
		connectEventSignal(event);
	}

	virtual void HandleDisconnectEvent(ENetEvent& event)
	{
		disconnectEventSignal(event);
	}

	virtual void HandleReceiveEvent(ENetEvent& event)
	{

	}

};
