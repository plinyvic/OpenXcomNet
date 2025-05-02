#pragma once

//#include <enet/enet.h>
#include <memory>
#include <queue>
#include "../../Network/NetEvent/NetEventReceive.h"
#include "../../Network/PacketFactory/PacketFactory.h"
#include "../../Network/NetworkController/OutboundPacket.h"

class NetEventbase;

template<typename TNetHostType>
class NetworkController
{
  public:
	std::unique_ptr<TNetHostType> host;

	bool shouldExit;
	bool isInitialized;

	std::queue<OutboundPacket> outboundPacketQueue;

	NetworkController(std::unique_ptr<TNetHostType>&& newHost) : host{std::move(newHost)}, shouldExit{false}, outboundPacketQueue{}, isInitialized{true}
	{
		host->SetPacketBuffer(outboundPacketQueue);
	}

	NetworkController() : shouldExit{false}, outboundPacketQueue{}, isInitialized{false}
	{
	}
	~NetworkController() = default;

	template<typename THeader, typename TData>
	inline void CreateOutboundPacket(THeader header, TData& data)
	{
		// should really have the host create the packet so they can decide what to do with it
		// but this is fine for now
		ENetPacket* packet = PacketFactory::MakeENetPacket(header, data);
		outboundPacketQueue.push(std::move(OutboundPacket(nullptr, 0, packet)));
	}

	template <typename THeader>
	inline void CreateOutboundPacketVoid(THeader header)
	{
		// should really have the host create the packet so they can decide what to do with it
		// but this is fine for now
		ENetPacket* packet = PacketFactory::MakeENetPacketVoid(header);
		outboundPacketQueue.push(std::move(OutboundPacket(nullptr, 0, packet)));
	}

	inline void SetHost(std::unique_ptr<TNetHostType>&& newHost)
	{
		isInitialized = newHost != nullptr;
		host = std::move(newHost);
	}

	// receive packets and add them to event queue to be processed this frame
	inline void PreThink()
	{
		NetworkProcessPackets();
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

	inline void NetworkSendOutboundPackets()
	{
		while (!outboundPacketQueue.empty())
		{
			OutboundPacket& outboundPacket = outboundPacketQueue.front();

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
			outboundPacketQueue.pop();
		}
		// we want to receive and send packets at seperate times. flush instead of host service.
		enet_host_flush(host->host);
	}
};
