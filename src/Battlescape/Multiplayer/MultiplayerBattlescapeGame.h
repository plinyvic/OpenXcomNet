#pragma once

#include "../BattlescapeGame.h"
#include <boost/signals2.hpp>

struct MultiplayerBattleAction;

class MultiplayerBattlescapeGame : public OpenXcom::BattlescapeGame
{

protected:

	boost::signals2::scoped_connection onReceivePushStateFromActionFrontConnection;
	boost::signals2::scoped_connection onReceivePushStateFromActionNextConnection;
	boost::signals2::scoped_connection onReceivePushStateFromActionBackConnection;

public:

	MultiplayerBattlescapeGame(OpenXcom::SavedBattleGame* save, OpenXcom::BattlescapeState* parentState);

	OpenXcom::BattleState* MakeBattleState(MultiplayerBattleAction& battleAction);

	void OnReceivePushStateFromActionFront(MultiplayerBattleAction& mpba);
	void OnReceivePushStateFromActionNext(MultiplayerBattleAction& mpba);
	void OnReceivePushStateFromActionBack(MultiplayerBattleAction& mpba);

	virtual void PushStateFromActionFront(OpenXcom::BattleState* state) override;
	virtual void PushStateFromActionNext(OpenXcom::BattleState* state) override;
	virtual void PushStateFromActionBack(OpenXcom::BattleState* state) override;

};
