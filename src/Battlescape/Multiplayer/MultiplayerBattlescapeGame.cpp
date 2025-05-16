#include "MultiplayerBattlescapeGame.h"

#include "../BattlescapeState.h"
#include "../BattleState.h"
#include "../../Engine/Game.h"
#include "../../Battlescape/UnitWalkBState.h"
#include "../../Battlescape/ProjectileFlyBState.h"
#include "../../Battlescape/UnitTurnBState.h"
#include "../../Battlescape/Pathfinding.h"
#include "../../Battlescape/PsiAttackBState.h"
#include "../UnitKneelState.h"

MultiplayerBattlescapeGame::MultiplayerBattlescapeGame(SavedBattleGame* save, BattlescapeState* parentState) : BattlescapeGame(save, parentState)
{
	onReceiveFlushBattleActionsConnection = _parentState->getGame()->GetNetHost()->BindToReceiveFlushBattleActionsEvent([this](MultiplayerBattleActionVector& mpbas)
																														{OnReceiveFlushBattleActions(mpbas);});
}

OpenXcom::BattleState* MultiplayerBattlescapeGame::MakeBattleState(MultiplayerBattleAction& multiplayerBattleAction)
{
	OpenXcom::BattleState* state = nullptr;
	OpenXcom::BattleAction battleAction = multiplayerBattleAction.MakeBattleAction(_parentState->getGame());
	switch (battleAction.type)
	{
	case OpenXcom::BattleActionType::BA_WALK:
		state = new OpenXcom::UnitWalkBState(this, battleAction);
		getSave()->getPathfinding()->calculate(battleAction.actor, battleAction.target, BAM_NORMAL);
		break;
	case OpenXcom::BattleActionType::BA_TURN:
		state = new OpenXcom::UnitTurnBState(this, battleAction);
		break;
	case OpenXcom::BattleActionType::BA_SNAPSHOT:
	case OpenXcom::BattleActionType::BA_AUTOSHOT:
	case OpenXcom::BattleActionType::BA_AIMEDSHOT:
	case OpenXcom::BattleActionType::BA_THROW:
		state = new OpenXcom::ProjectileFlyBState(this, battleAction);
		break;
	case OpenXcom::BattleActionType::BA_PANIC:
	case OpenXcom::BattleActionType::BA_MINDCONTROL:
	case OpenXcom::BattleActionType::BA_USE:
		state = new OpenXcom::PsiAttackBState(this, battleAction);
		break;
	case OpenXcom::BattleActionType::BA_KNEEL:
		state = new OpenXcom::UnitKneelState(this, battleAction);
		break;
	default:
		break;
	}
	return state;
}

void MultiplayerBattlescapeGame::OnReceiveFlushBattleActions(MultiplayerBattleActionVector& mpbActions)
{
	// clear any current states (there shouldnt be any)
	_states.clear();
	outboundActions.clear();
	RNG::setSeed(mpbActions.rngSeed);
	// add states in order
	for(auto mpba : mpbActions.orderedActions)
	{
		_states.push_back(MakeBattleState(mpba));
	}
	if (!_states.empty())
	{
		getSave()->setSelectedUnit(_states.front()->getAction().actor);
		_states.front()->init();
	}

	// init back?
}

void MultiplayerBattlescapeGame::FlushBattleActions()
{
	if (!outboundActions.empty())
	{
		// move list to vector
		std::vector<MultiplayerBattleAction> actions{std::make_move_iterator(std::begin(outboundActions)), std::make_move_iterator(std::end(outboundActions))};

		// reinitialize list since we moved its contents
		outboundActions = std::list<MultiplayerBattleAction>();

		MultiplayerBattleActionVector outboundData{std::move(actions), startingSeed};
		_parentState->getGame()->GetNetworkControllerMutable().CreateOutboundPacket(EXcomNetEventType::FlushBattleActions, outboundData);
	}
	// if actions are empty, why would we do anything?
}

void MultiplayerBattlescapeGame::primaryAction(Position pos)
{
	BattlescapeGame::primaryAction(pos);

	FlushBattleActions();
}

void MultiplayerBattlescapeGame::secondaryAction(Position pos)
{
	BattlescapeGame::secondaryAction(pos);

	FlushBattleActions();
}

bool MultiplayerBattlescapeGame::KneelAction(OpenXcom::BattleUnit* bu)
{
	bool toReturn = kneel(bu);
	FlushBattleActions();
	return toReturn;
}


void MultiplayerBattlescapeGame::PushStateFromActionFront(BattleState* state)
{
	if (outboundActions.empty())
	{
		startingSeed = RNG::getSeed();
	}
	// add to network event queue
	MultiplayerBattleAction mpba{state->getAction()};
	outboundActions.push_front(mpba);

	BattlescapeGame::PushStateFromActionFront(state);
}

void MultiplayerBattlescapeGame::PushStateFromActionNext(BattleState* state, bool doNotInit)
{
	if (outboundActions.empty())
	{
		startingSeed = RNG::getSeed();
	}
	// add to network event queue
	MultiplayerBattleAction mpba{state->getAction()};
	if (outboundActions.empty())
	{
		outboundActions.push_front(mpba);
	}
	else
	{
		outboundActions.insert(++outboundActions.begin(), mpba);
	}

	BattlescapeGame::PushStateFromActionNext(state, doNotInit);
}

void MultiplayerBattlescapeGame::PushStateFromActionBack(BattleState* state, bool doNotInit)
{
	if (outboundActions.empty())
	{
		startingSeed = RNG::getSeed();
	}
	// add to network event queue
	MultiplayerBattleAction mpba{state->getAction()};
	if (outboundActions.empty())
	{
		outboundActions.push_front(mpba);
	}
	else
	{
		outboundActions.push_back(mpba);
	}

	BattlescapeGame::PushStateFromActionBack(state, doNotInit);
}
