#pragma once

//#include <enet/enet.h>
#include <memory>
#include <queue>
#include "../../Network/NetEvent/NetEventReceive.h"
#include "../../Network/PacketFactory/PacketFactory.h"
#include "../../Network/NetworkController/OutboundPacket.h"

class NetHost;
class NetEventbase;

//class NetworkController
//{
//public:
//
//	std::unique_ptr<NetHost> host;
//
//	bool shouldExit;
//	bool isInitialized;
//
//	std::queue<std::unique_ptr<NetEventBase>> netEventQueue;
//	std::shared_ptr<std::queue<OutboundPacket>> outboundPacketQueue;
//
//	NetworkController(std::unique_ptr<NetHost>&& newHost);
//	NetworkController();
//	~NetworkController();
//
//	void SetHost(std::unique_ptr<NetHost>& newHost);
//
//	// receive packets and add them to event queue to be processed this frame
//	void PreThink();
//
//	// send packets queued from this frame
//	void PostThink();
//
//protected:
//
//	void HandleConnectEvent(ENetEvent& connectEvent);
//	void HandleDisconnectEvent(ENetEvent& disconnectEvent);
//	void HandleReceiveEvent(ENetEvent& receiveEvent);
//
//	// process application specific logic, such as input and displaying text
//	void ApplicationProcessLogic();
//
//	// process all received packets as events and destroy them.
//	void NetworkProcessPackets();
//
//	// process network events
//	void NetworkProcessEvents();
//
//	void NetworkSendOutboundPackets();
//
//	template<typename TPacketPayload, void(NetHost::* THostFunction)(TNetEventReceive<PacketData<TPacketPayload>>&)>
//	void AddEventToStack(ENetPacket& packet);
//};
//
//template<typename TPacketPayload, void(NetHost::* THostFunction)(TNetEventReceive<PacketData<TPacketPayload>>&)>
//inline void NetworkController::AddEventToStack(ENetPacket& packet)
//{
//	netEventQueue.push(std::make_unique<TNetEventReceiveFunc<PacketData<TPacketPayload>, THostFunction>>(PacketFactory::GetData<TPacketPayload>(packet)));
//}

template<typename TNetHostType>
class NetworkController
{
  public:
	std::unique_ptr<TNetHostType> host;

	bool shouldExit;
	bool isInitialized;

	std::queue<std::unique_ptr<NetEventBase<TNetHostType>>> netEventQueue;
	std::shared_ptr<std::queue<OutboundPacket> > outboundPacketQueue;

	NetworkController(std::unique_ptr<TNetHostType>&& newHost) : host{std::move(newHost)}, shouldExit{false}, netEventQueue{}, outboundPacketQueue{std::make_shared<std::queue<OutboundPacket> >()}, isInitialized{true}
	{
		host->SetPacketBuffer(outboundPacketQueue);
	}

	NetworkController() : shouldExit{false}, netEventQueue{}, outboundPacketQueue{std::make_shared<std::queue<OutboundPacket> >()}, isInitialized{false}
	{
	}
	~NetworkController() = default;

	inline void SetHost(std::unique_ptr<TNetHostType>&& newHost)
	{
		isInitialized = newHost != nullptr;
		host = std::move(newHost);
	}

	// receive packets and add them to event queue to be processed this frame
	inline void PreThink()
	{
		NetworkProcessPackets();
		NetworkProcessEvents();
	}

	// send packets queued from this frame
	inline void PostThink()
	{
		NetworkSendOutboundPackets();
		enet_host_service(host->host, nullptr, 0);
	}

protected:

	// process all received packets as events and destroy them.
	inline void NetworkProcessPackets()
	{
		host->HandleENetEvents();
	}

	// process network events
	inline void NetworkProcessEvents()
	{
		// handle each event in order
		while (!netEventQueue.empty())
		{
			std::unique_ptr<NetEventBase<TNetHostType>> currentEvent = move(netEventQueue.front());
			netEventQueue.pop();
			currentEvent->HandleEvent(*host);
		}
	}

	inline void NetworkSendOutboundPackets()
	{
		// while (!outboundPacketQueue.empty())
		while (!outboundPacketQueue->empty())
		{
			OutboundPacket& outboundPacket = outboundPacketQueue->front();

			if (outboundPacket.peer == nullptr)
			{
				// assume we want to broadcast to all peers.
				enet_host_broadcast(host->host, outboundPacket.channelId, outboundPacket.packet);
			}
			else
			{
				// otherwise just to the target peer.
				enet_peer_send(outboundPacket.peer, outboundPacket.channelId, outboundPacket.packet);
			}
			outboundPacketQueue->pop();
		}
		// we want to receive and send packets at seperate times. flush instead of host service.
		enet_host_flush(host->host);
	}

	template <typename TPacketPayload, void(TNetHostType::*THostFunction)(TNetEventReceive<PacketData<TPacketPayload>, TNetHostType>&)>
	inline void AddEventToStack(ENetPacket& packet)
	{
		netEventQueue.push(std::make_unique<TNetEventReceiveFunc<PacketData<TPacketPayload>, TNetHostType, THostFunction> >(PacketFactory::GetData<TPacketPayload>(packet)));
	}
};

//template <typename TPacketPayload, void(TNetHostType::*THostFunction)(TNetEventReceive<PacketData<TPacketPayload>, TNetHostType>&)>
//inline void NetworkController::AddEventToStack(ENetPacket& packet)
//{
//	netEventQueue.push(std::make_unique<TNetEventReceiveFunc<PacketData<TPacketPayload>, TNetHostType, THostFunction> >(PacketFactory::GetData<TPacketPayload>(packet)));
//}
