#include "MultiplayerBattlescapeGame.h"

#include "../BattlescapeState.h"
#include "../BattleState.h"
#include "../../Engine/Game.h"
#include "MultiplayerBattleAction.h"
#include "../../Battlescape/UnitWalkBState.h"
#include "../../Battlescape/Pathfinding.h"

MultiplayerBattlescapeGame::MultiplayerBattlescapeGame(SavedBattleGame* save, BattlescapeState* parentState) : BattlescapeGame(save, parentState)
{
	// bind to events
	onReceivePushStateFromActionFrontConnection = _parentState->getGame()->GetNetHost()->BindToReceivePushBattleActionFrontEvent([this](MultiplayerBattleAction& mpba)
																																 {OnReceivePushStateFromActionFront(mpba);});
	onReceivePushStateFromActionNextConnection = _parentState->getGame()->GetNetHost()->BindToReceivePushBattleActionNextEvent([this](MultiplayerBattleAction& mpba)
																																{OnReceivePushStateFromActionNext(mpba);});
	onReceivePushStateFromActionBackConnection = _parentState->getGame()->GetNetHost()->BindToReceivePushBattleActionBackEvent([this](MultiplayerBattleAction& mpba)
																																{OnReceivePushStateFromActionBack(mpba);});
}

OpenXcom::BattleState* MultiplayerBattlescapeGame::MakeBattleState(MultiplayerBattleAction& battleAction)
{
	// set rng seed so that actions have parity on all clients.
	RNG::setSeed(battleAction.rngSeed);
	// set selected actor
	getSave()->setSelectedUnit(battleAction.actor);
	OpenXcom::BattleState* state = nullptr;
	switch (battleAction.type)
	{
	case OpenXcom::BattleActionType::BA_WALK:
		state = new OpenXcom::UnitWalkBState(this, battleAction);
		getSave()->getPathfinding()->calculate(battleAction.actor, battleAction.target, BAM_NORMAL);
		break;
	default:
		break;
	}

	return state;
}

void MultiplayerBattlescapeGame::OnReceivePushStateFromActionFront(MultiplayerBattleAction& mpba)
{
	statePushFront(MakeBattleState(mpba));
}

void MultiplayerBattlescapeGame::OnReceivePushStateFromActionNext(MultiplayerBattleAction& mpba)
{
	statePushNext(MakeBattleState(mpba));
}

void MultiplayerBattlescapeGame::OnReceivePushStateFromActionBack(MultiplayerBattleAction& mpba)
{
	statePushBack(MakeBattleState(mpba));
}

void MultiplayerBattlescapeGame::PushStateFromActionFront(BattleState* state)
{
	// add to network event queue
	MultiplayerBattleAction mpba{_parentState->getGame(), RNG::getSeed(), state->getAction()};
	_parentState->getGame()->GetNetworkControllerMutable().CreateOutboundPacket(EXcomNetEventType::PushBattleActionFront, mpba);

	BattlescapeGame::PushStateFromActionFront(state);
}

void MultiplayerBattlescapeGame::PushStateFromActionNext(BattleState* state)
{
	// add to network event queue
	MultiplayerBattleAction mpba{_parentState->getGame(), RNG::getSeed(), state->getAction()};
	_parentState->getGame()->GetNetworkControllerMutable().CreateOutboundPacket(EXcomNetEventType::PushBattleActionNext, mpba);

	BattlescapeGame::PushStateFromActionNext(state);
}

void MultiplayerBattlescapeGame::PushStateFromActionBack(BattleState* state)
{
	// add to network event queue
	MultiplayerBattleAction mpba{_parentState->getGame(), RNG::getSeed(), state->getAction()};
	_parentState->getGame()->GetNetworkControllerMutable().CreateOutboundPacket(EXcomNetEventType::PushBattleActionBack, mpba);

	BattlescapeGame::PushStateFromActionBack(state);
}
