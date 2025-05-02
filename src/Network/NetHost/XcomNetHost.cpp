#include "XcomNetHost.h"
#include "../PacketFactory/PacketFactory.h"
#include <cereal/types/string.hpp>

void XcomNetHost::HandleReceiveEvent(ENetEvent& event)
{
	if (event.packet == nullptr)
	{
		return;
	}

	EXcomNetEventType header = PacketFactory::GetHeader<EXcomNetEventType>(*event.packet);
	switch (header)
	{
	case EXcomNetEventType::ReceiveHostSave:
	{
		std::string data = PacketFactory::GetData<EXcomNetEventType, std::string>(*event.packet).messageData;
		receiveHostSaveSignal(data);
		break;
	}
	case EXcomNetEventType::ReceiveClientReady:
		receiveClientReadySignal();
		break;
	case EXcomNetEventType::ReceiveStartMatch:
		receiveStartMatchSignal();
		break;
	case EXcomNetEventType::NET_ERROR:
		break;
	default:
		break;
	}
}
