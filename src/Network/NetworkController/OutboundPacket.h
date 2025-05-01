#pragma once

#include "../../enet/enet.h"
#include <cstdint>

struct OutboundPacket
{
	ENetPeer* peer;
	uint8_t channelId;
	ENetPacket* packet;

	OutboundPacket(ENetPeer* inPeer, uint8_t inChannelId, ENetPacket* inPacket) : peer{ inPeer }, channelId{ inChannelId }, packet{ inPacket } {}
};
