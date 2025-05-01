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
		BattlescapeGenerator bgen = MakeBattlescapeGeneratorFromNewBattleState();

		bgen.run();

		_game->popState();
		_game->popState();
		_game->pushState(new MultiplayerWaitConnectState(EHostType::Host, "127.0.0.1"));

		/*_game->popState();
		_game->popState();
		_game->pushState(new MultiplayerWaitConnectState());
		_craft = 0;*/
	}
}


