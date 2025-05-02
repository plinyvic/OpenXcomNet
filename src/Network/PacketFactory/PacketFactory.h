#pragma once
#include <cereal/archives/portable_binary.hpp>
#include "../../Network/NetEvent/NetEventReceive.h"
#include <sstream>

template <typename THeaderType, typename TMessageType>
struct PacketData
{
	THeaderType headerType;
	TMessageType messageData;

	template<class Archive>
	inline void serialize(Archive& archive)
	{
		archive(headerType, messageData);
	}

	PacketData() {}
	PacketData(THeaderType inHeader, const TMessageType& inMessageData) : headerType(inHeader), messageData(inMessageData) {}
	PacketData(THeaderType inHeader, TMessageType&& inMessageData) : headerType{inHeader}, messageData {std::move(inMessageData)} {}
};

class PacketFactory
{
public:

	template <typename THeader>
	static ENetPacket* MakeENetPacketVoid(THeader header);

	template<typename THeader, typename TMessage>
	static ENetPacket* MakeENetPacket(PacketData<THeader, TMessage>& data);

	template <typename THeader, typename TMessage>
	static ENetPacket* MakeENetPacket(THeader header, TMessage& message);

	template<typename THeader, typename TMessage>
	static PacketData<THeader, TMessage> GetData(ENetPacket& packet);

	template<typename THeader>
	static THeader GetHeader(ENetPacket& packet);

};

template <typename THeader>
inline ENetPacket* PacketFactory::MakeENetPacketVoid(THeader header)
{
	std::stringstream stream;
	{
		cereal::PortableBinaryOutputArchive oarchive(stream);
		oarchive(header);
	}

	return enet_packet_create(stream.str().data(), stream.str().size(), ENET_PACKET_FLAG_RELIABLE);
}

template <typename THeader, typename TMessage>
inline ENetPacket* PacketFactory::MakeENetPacket(PacketData<THeader, TMessage>& data)
{
	std::stringstream stream;
	{
		cereal::PortableBinaryOutputArchive oarchive(stream);
		oarchive(data);
	}

	return enet_packet_create(stream.str().data(), stream.str().size(), ENET_PACKET_FLAG_RELIABLE);
}

template <typename THeader, typename TMessage>
inline ENetPacket* PacketFactory::MakeENetPacket(THeader header, TMessage& message)
{
	std::stringstream stream;
	{
		cereal::PortableBinaryOutputArchive oarchive(stream);
		oarchive(header, message);
	}

	return enet_packet_create(stream.str().data(), stream.str().size(), ENET_PACKET_FLAG_RELIABLE);
}

template <typename THeader, typename TMessage>
inline PacketData<THeader, TMessage> PacketFactory::GetData(ENetPacket& packet)
{
	PacketData<THeader, TMessage> data;
	std::stringstream stream = std::stringstream(std::string(reinterpret_cast<char*>(packet.data), packet.dataLength));
	{
		cereal::PortableBinaryInputArchive iarchive(stream);
		
		iarchive(data);
	}

	return data;
}

template <typename THeader>
THeader PacketFactory::GetHeader(ENetPacket& packet)
{
	THeader dataType;
	std::stringstream stream = std::stringstream(std::string(reinterpret_cast<char*>(packet.data), packet.dataLength));
	{
		cereal::PortableBinaryInputArchive iarchive(stream);
		iarchive(dataType);
	}

	return dataType;
}
