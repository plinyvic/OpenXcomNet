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

template<typename T>
struct PacketData;

template <typename TDataType, typename TNetHostType>
class TNetEventReceive;

class NetHost
{
public:

	ENetHost* host;

protected:

	std::shared_ptr<std::queue<OutboundPacket>> outboundPacketBuffer;

	ENetAddress address;

private:

	boost::signals2::signal<void(ENetEvent&)> connectEventSignal;
	boost::signals2::signal<void(ENetEvent&)> disconnectEventSignal;

public:

	virtual ~NetHost();

	void SetPacketBuffer(const std::shared_ptr<std::queue<OutboundPacket>>& controllerBuffer);

	void HandleENetEvents();

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, ENetEvent&> inline boost::signals2::connection BindToOnConnectEvent(TCallable&& function)
	{
		return connectEventSignal.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, ENetEvent&> inline boost::signals2::connection BindToOnDisconnectEvent(TCallable&& function)
	{
		return disconnectEventSignal.connect(std::forward<TCallable>(function));
	}

	/*template <typename TCallable>
	requires CallableWithSignature<TCallable, void, ENetEvent&> inline boost::signals2::connection BindToOnReceiveEvent(TCallable&& function)
	{
		return receiveEventSignal.connect(std::forward<TCallable>(function));
	}*/

protected:

	virtual void HandleConnectEvent(ENetEvent& event);
	virtual void HandleDisconnectEvent(ENetEvent& event);

private:

	virtual void HandleReceiveEvent(ENetEvent& event) = 0;

};
