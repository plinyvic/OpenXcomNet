#pragma once
#include <cereal/archives/portable_binary.hpp>
#include "../../Network/NetEvent/NetEventReceive.h"
#include <sstream>

template<typename T>
struct PacketData
{
public:
	ENetEventReceiveDataType messageType;
	T messageData;

	template<class Archive>
	inline void serialize(Archive& archive)
	{
		archive(messageType, messageData);
	}

	PacketData() {}
	PacketData(ENetEventReceiveDataType inMessageType, const T& inMessageData) : messageType(inMessageType), messageData(inMessageData) {}
};

class PacketFactory
{
public:

	template<typename T>
	static ENetPacket* MakePacket(PacketData<T>& data);

	template<typename T>
	static PacketData<T> GetData(ENetPacket& packet);

	static ENetEventReceiveDataType GetHeader(ENetPacket& packet);

};

template<typename T>
inline ENetPacket* PacketFactory::MakePacket(PacketData<T>& data)
{
	std::stringstream stream;
	{
		cereal::PortableBinaryOutputArchive oarchive(stream);
		oarchive(data);
	}

	return enet_packet_create(stream.str().data(), stream.str().size(), ENET_PACKET_FLAG_RELIABLE);
}

template<typename T>
inline PacketData<T> PacketFactory::GetData(ENetPacket& packet)
{
	PacketData<T> data;
	std::stringstream stream = std::stringstream(std::string(reinterpret_cast<char*>(packet.data), packet.dataLength));
	{
		cereal::PortableBinaryInputArchive iarchive(stream);
		
		iarchive(data);
	}

	return data;
}
