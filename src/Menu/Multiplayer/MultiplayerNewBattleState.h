#pragma once
#include "../NewBattleState.h"

namespace OpenXcom
{
	class MultiplayerNewBattleState : public NewBattleState
	{
	public:
		MultiplayerNewBattleState();

		// override base NewBattleState to host and wait for player
		virtual void btnOkClick(Action* action) override;
	};
}

