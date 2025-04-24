#pragma once

//#include <enet/enet.h>
#include <memory>
#include <queue>
#include "../../Network/NetEvent/NetEventReceive.h"
#include "../../Network/PacketFactory/PacketFactory.h"
#include "../../Network/NetworkController/OutboundPacket.h"

class NetHost;
class NetEventbase;

class NetworkController
{
public:

	std::unique_ptr<NetHost> host;

	bool shouldExit;

	std::queue<std::unique_ptr<NetEventBase>> netEventQueue;
	std::shared_ptr<std::queue<OutboundPacket>> outboundPacketQueue;

	void Loop();

	//NetworkController(unique_ptr<NetHost> newHost) : host(move(newHost)), shouldExit(false), netEventQueue(), outboundPacketQueue(queue<OutboundPacket>()) {};
	/*NetworkController(std::unique_ptr<NetHost> newHost) : host(std::move(newHost)), shouldExit(false), netEventQueue(), outboundPacketQueue(std::make_shared<TSQueue<OutboundPacket>>())
	{
		host->SetPacketBuffer(outboundPacketQueue);
	}*/

	NetworkController(std::unique_ptr<NetHost>&& newHost);

protected:

	void HandleConnectEvent(ENetEvent& connectEvent);
	void HandleDisconnectEvent(ENetEvent& disconnectEvent);
	void HandleReceiveEvent(ENetEvent& receiveEvent);

	// process application specific logic, such as input and displaying text
	void ApplicationProcessLogic();

	// process all received packets as events and destroy them.
	void NetworkProcessPackets();

	// process network events
	void NetworkProcessEvents();

	void NetworkSendOutboundPackets();

	template<typename TPacketPayload, void(NetHost::* THostFunction)(TNetEventReceive<PacketData<TPacketPayload>>&)>
	void AddEventToStack(ENetPacket& packet);
};

template<typename TPacketPayload, void(NetHost::* THostFunction)(TNetEventReceive<PacketData<TPacketPayload>>&)>
inline void NetworkController::AddEventToStack(ENetPacket& packet)
{
	netEventQueue.push(std::make_unique<TNetEventReceiveFunc<PacketData<TPacketPayload>, THostFunction>>(PacketFactory::GetData<TPacketPayload>(packet)));
}
