#pragma once

#include "../BattlescapeGame.h"
#include <boost/signals2.hpp>
#include "MultiplayerBattleAction.h"
#include <list>

class MultiplayerBattlescapeGame : public OpenXcom::BattlescapeGame
{

protected:

	boost::signals2::scoped_connection onReceiveFlushBattleActionsConnection;

	uint64_t startingSeed;

	std::list<MultiplayerBattleAction> outboundActions;

public:

	MultiplayerBattlescapeGame(OpenXcom::SavedBattleGame* save, OpenXcom::BattlescapeState* parentState);

	OpenXcom::BattleState* MakeBattleState(MultiplayerBattleAction& multiplayerBattleAction);

	void OnReceiveFlushBattleActions(MultiplayerBattleActionVector& mpbActions);

	void FlushBattleActions();

	// OpenXcom::BattlescapeGame overrides

	virtual void primaryAction(Position pos) override;
	virtual void secondaryAction(Position pos) override;
	virtual void moveUpDown(BattleUnit* unit, int dir) override;
	virtual bool KneelAction(OpenXcom::BattleUnit* bu);

	virtual void PushStateFromActionFront(OpenXcom::BattleState* state) override;
	virtual void PushStateFromActionNext(OpenXcom::BattleState* state, bool doNotInit = false) override;
	virtual void PushStateFromActionBack(OpenXcom::BattleState* state, bool doNotInit = false) override;

};
