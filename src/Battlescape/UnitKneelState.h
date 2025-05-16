#pragma once

#include "BattleState.h"

namespace OpenXcom
{
class BattlescapeGame;
struct BattleAction;

class UnitKneelState : public BattleState
{
public:
	UnitKneelState(BattlescapeGame* parent, const BattleAction& action);
	void init() override;

	/// Get whether or not this crouch was successful. Populated after init().
	bool GetSuccess() const { return _success; }

private:
	bool _success;
};

}

