#pragma once

#include <cstdint>

struct _ENetPeer;
typedef _ENetPeer ENetPeer;

struct _ENetPacket;
typedef _ENetPacket ENetPacket;

struct OutboundPacket
{
	ENetPeer* peer;
	uint8_t channelId;
	ENetPacket* packet;

	OutboundPacket(ENetPeer* inPeer, uint8_t inChannelId, ENetPacket* inPacket) : peer{ inPeer }, channelId{ inChannelId }, packet{ inPacket } {}
};
