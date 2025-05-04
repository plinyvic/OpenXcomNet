#include "XcomNetHost.h"
#include "../PacketFactory/PacketFactory.h"
#include <cereal/types/string.hpp>
#include "../../Battlescape/Multiplayer/MultiplayerBattleAction.h"
#include "../../Battlescape/BattlescapeState.h"
#include "../../Battlescape/UnitWalkBState.h"

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
	case EXcomNetEventType::PushBattleActionFront:
	{
		MultiplayerBattleAction battleAction = PacketFactory::GetData<EXcomNetEventType, MultiplayerBattleAction>(*event.packet, game).messageData;
		receivePushBattleActionFrontSignal(battleAction);
	}
		break;
	case EXcomNetEventType::PushBattleActionNext:
	{
		MultiplayerBattleAction battleAction = PacketFactory::GetData<EXcomNetEventType, MultiplayerBattleAction>(*event.packet, game).messageData;
		receivePushBattleActionNextSignal(battleAction);
	}
		break;
	case EXcomNetEventType::PushBattleActionBack:
	{
		MultiplayerBattleAction battleAction = PacketFactory::GetData<EXcomNetEventType, MultiplayerBattleAction>(*event.packet, game).messageData;
		receivePushBattleActionBack(battleAction);
	}
		break;
	case EXcomNetEventType::NET_ERROR:
		break;
	default:
		break;
	}
}



void XcomNetHost::SetGame(OpenXcom::Game* inGame)
{
	game = inGame;
}
