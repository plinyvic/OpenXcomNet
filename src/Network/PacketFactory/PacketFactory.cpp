#include "PacketFactory.h"

//template<typename T>
//ENetPacket* PacketFactory::MakePacket(PacketData<T> data)
//{
//	//ENetPacket newPacket = enet_packet_create()
//
//	stringstream stream;
//	{
//		cereal::PortableBinaryOutputArchive oarchive(stream);
//		oarchive(data);
//	}
//
//	return enet_packet_create(stream.str().data(), stream.str().size(), ENET_PACKET_FLAG_RELIABLE);
//}
//
//template<typename T>
//PacketData<T> PacketFactory::GetData(ENetPacket* packet)
//{
//	PacketData<T> data;
//	stringstream stream = stringstream(string(reinterpret_cast<char*>(packet->data), packet->dataLength));
//	{
//		cereal::PortableBinaryInputArchive iarchive(stream);
//		iarchive(data);
//	}
//
//	return data;
//}

ENetEventReceiveDataType PacketFactory::GetHeader(ENetPacket& packet)
{
	ENetEventReceiveDataType dataType = ENetEventReceiveDataType::NONE;
	std::stringstream stream = std::stringstream(std::string(reinterpret_cast<char*>(packet.data), packet.dataLength));
	{
		cereal::PortableBinaryInputArchive iarchive(stream);
		iarchive(dataType);
	}

	return dataType;
}