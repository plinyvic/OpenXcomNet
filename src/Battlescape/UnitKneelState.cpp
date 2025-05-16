#include "UnitKneelState.h"

#include "../Savegame/BattleUnit.h"
#include "../Savegame/SavedBattleGame.h"
#include "BattlescapeGame.h"
#include "TileEngine.h"
#include "BattlescapeState.h"

OpenXcom::UnitKneelState::UnitKneelState(BattlescapeGame* parent, const BattleAction& action) : BattleState(parent, action), _success{false}
{
	
}

void OpenXcom::UnitKneelState::init()
{
	int tu = _action.actor->getKneelChangeCost();
	if (_action.actor->getArmor()->allowsKneeling(_action.actor->getType() == "SOLDIER") && !_action.actor->isFloating() && ((!_action.actor->isKneeled() && _parent->getSave()->getKneelReserved()) || _parent->checkReservedTU(_action.actor, tu, 0)))
	{
		_action.Time = tu;
		if (_action.spendTU())
		{
			_action.actor->kneel(!_action.actor->isKneeled());
			// kneeling or standing up can reveal new terrain or units. I guess.
			_parent->getTileEngine()->calculateFOV(_action.actor->getPosition(), 1, false); // Update unit FOV for everyone through this position, skip tiles.
			_parent->GetParentState()->updateSoldierInfo();                          // This also updates the tile FOV of the unit, hence why it's skipped above.
			_parent->getTileEngine()->checkReactionFire(_action.actor, _action);
			_success = true;
		}
		else
		{
			_parent->GetParentState()->warning("STR_NOT_ENOUGH_TIME_UNITS");
		}
	}
	else
	{
		_success = false;
	}
	_parent->popState();
}
