#include "PacketFactory.h"

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
