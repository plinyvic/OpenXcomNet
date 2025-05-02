#include "MultiplayerNewBattleState.h"
#include "../../Interface/Text.h"
#include "../../Engine/Game.h"
#include "../../Battlescape/BriefingState.h"
#include "MultiplayerWaitConnectState.h"

namespace OpenXcom
{
	MultiplayerNewBattleState::MultiplayerNewBattleState()
	{
		// change text of NewBattleState::_textTitle
		_txtTitle->setText("HOST NEW MISSION");
	}

	void MultiplayerNewBattleState::btnOkClick(Action* action)
	{
		save();
		BattlescapeGenerator bgen = MakeBattlescapeGeneratorFromNewBattleState();

		bgen.run();

		std::string battleStateString = SaveToString();

		_game->popState();
		_game->popState();
		_game->pushState(new MultiplayerWaitConnectState(EHostType::Host, "127.0.0.1", battleStateString));
	}
}


