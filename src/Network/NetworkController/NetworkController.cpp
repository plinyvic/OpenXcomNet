#include "NetworkController.h"
#include <iostream>
#include "../../Network/NetHost/NetHost.h"

void NetworkController::Loop()
{
    do
    {
        NetworkProcessPackets();
        NetworkProcessEvents();
        ApplicationProcessLogic();
        NetworkSendOutboundPackets();
        enet_host_service(host->host, nullptr, 0);
    } while (!shouldExit);
}

void NetworkController::ApplicationProcessLogic()
{
    host->Application();
}

void NetworkController::NetworkProcessPackets()
{
    ENetEvent event;
    while (enet_host_service(host->host, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            HandleConnectEvent(event);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            HandleDisconnectEvent(event);
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            HandleReceiveEvent(event);
            break;
        }
    }
}

void NetworkController::NetworkProcessEvents()
{
    // handle each event in order
    while (!netEventQueue.empty())
    {
        std::unique_ptr<NetEventBase> currentEvent = move(netEventQueue.front());
        netEventQueue.pop();
        currentEvent->HandleEvent(*host);
    }
}

void NetworkController::NetworkSendOutboundPackets()
{
    //while (!outboundPacketQueue.empty())
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
    }
    // we want to receive and send packets at seperate times. flush instead of host service.
    outboundPacketQueue->pop();
    enet_host_flush(host->host);
}

NetworkController::NetworkController(std::unique_ptr<NetHost>&& newHost) : host{ std::move(newHost) }, shouldExit{ false }, netEventQueue{}, outboundPacketQueue{std::make_shared<std::queue<OutboundPacket>>()}
{
    host->SetPacketBuffer(outboundPacketQueue);
}

void NetworkController::HandleConnectEvent(ENetEvent& connectEvent)
{
    std::cout << "A new client connected from " << connectEvent.peer->address.host << ":" << connectEvent.peer->address.port << std::endl;
}

void NetworkController::HandleDisconnectEvent(ENetEvent& disconnectEvent)
{
    std::cout << "A client disconnected." << std::endl;
}

void NetworkController::HandleReceiveEvent(ENetEvent& receiveEvent)
{
    std::cout << "Packet received from " << receiveEvent.peer->address.host << ":" << receiveEvent.peer->address.port << std::endl;

    ENetEventReceiveDataType header = PacketFactory::GetHeader(*receiveEvent.packet);

    switch (header)
    {
    case ENetEventReceiveDataType::ChatMessage:
        
        break;
    case ENetEventReceiveDataType::ERROR_INVALID_BYTES:
        // crash and burn???
        break;
    default:
        break;
    }

    enet_packet_destroy(receiveEvent.packet);
}
