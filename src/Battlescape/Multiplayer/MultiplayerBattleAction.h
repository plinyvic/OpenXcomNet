#pragma once
#include <string>
#include <vector>
#include <list>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
#include "../BattlescapeGame.h"
#include "../../Mod/RuleItem.h"
#include "../../Mod/RuleSkill.h"
#include "../../Savegame/BattleItem.h"
#include "../../Engine/Game.h"
#include "../../Savegame/SavedGame.h"
#include "../../Savegame/SavedBattleGame.h"
#include "../../Savegame/BattleUnit.h"
#include "../../Battlescape/Position.h"

namespace OpenXcom
{
	// serializers needed for MultiplayerBattleAction

	// Position serializer
	template <class TArchive>
	void serialize(TArchive& archive, Position& p)
	{
		archive(p.x, p.y, p.z);
	}
}

using namespace OpenXcom;

// Battle action made to be serialized/deserialized over the network.
struct MultiplayerBattleAction
{
	// OpenXcom::RuleItemUseRuleBase
	int16_t time;
	int16_t energy;
	int16_t morale;
	int16_t health;
	int16_t stun;
	int16_t mana;

	// OpenXcom::BattleActionCost fields
	BattleActionType type;
	int actorId;
	int weaponId;
	std::string skillRulesType;

	// OpenXcom::BattleAction fields
	Position target;
	std::list<Position> waypoints;
	bool targeting;
	int value;
	std::string result;
	bool strafe;
	bool run;
	bool sneak;
	bool ignoreSpottedEnemies;
	bool kneel;
	int diff;
	int autoShotCounter;
	Position cameraPosition;
	bool desperate; 
	int finalFacing;
	bool finalAction;
	int number;                                                    
	bool sprayTargeting;                                           
	BattleActionOrigin relativeOrigin;                             
	int terrainMeleeTilePart;

	MultiplayerBattleAction() = default;

	MultiplayerBattleAction(const BattleAction& ba) : time{ ba.Time }, energy{ ba.Energy }, morale{ ba.Morale }, health{ ba.Health }, stun{ ba.Stun }, mana{ ba.Mana },
		type{ ba.type }, target{ ba.target }, waypoints{ ba.waypoints }, targeting{ ba.targeting }, value{ ba.value }, result{ ba.result }, strafe{ ba.strafe }, run{ ba.run },
		sneak{ ba.sneak }, ignoreSpottedEnemies{ ba.ignoreSpottedEnemies }, kneel{ ba.kneel }, diff{ ba.diff }, autoShotCounter{ ba.autoShotCounter },
		cameraPosition{ ba.cameraPosition }, desperate{ ba.desperate }, finalFacing{ ba.finalFacing }, finalAction{ ba.finalAction },
		number{ ba.number }, sprayTargeting{ ba.sprayTargeting }, relativeOrigin{ ba.relativeOrigin }, terrainMeleeTilePart{ ba.terrainMeleeTilePart }
	{
		// we get these unique ids. they should be stably on all clients.
		actorId = ba.actor != nullptr ? ba.actor->getId() : -1;
		weaponId = ba.weapon != nullptr ? ba.weapon->getId() : -1;
		skillRulesType = ba.skillRules != nullptr ? ba.skillRules->getType() : "";
	}

	BattleAction MakeBattleAction(Game* game)
	{
		BattleAction ba;
		ba.Time = time;
		ba.Energy = energy;
		ba.Morale = morale;
		ba.Health = health;
		ba.Stun = stun;
		ba.Mana = mana;
		ba.actor = nullptr;
		ba.weapon = nullptr;
		ba.skillRules = nullptr;
		ba.type = type;
		ba.target = target;
		ba.waypoints = waypoints;
		ba.targeting = targeting;
		ba.value = value;
		ba.result = result;
		ba.strafe = strafe;
		ba.run = run;
		ba.sneak = sneak;
		ba.ignoreSpottedEnemies = ignoreSpottedEnemies;
		ba.kneel = kneel;
		ba.diff = diff;
		ba.autoShotCounter = autoShotCounter;
		ba.cameraPosition = cameraPosition;
		ba.desperate = desperate;
		ba.finalFacing = finalFacing;
		ba.finalAction = finalAction;
		ba.number = number;
		ba.sprayTargeting = sprayTargeting;
		ba.relativeOrigin = relativeOrigin;
		ba.terrainMeleeTilePart = terrainMeleeTilePart;

		// get unit from game battlesave based on ID.
		if (actorId != -1)
		{
			std::vector<OpenXcom::BattleUnit*>* saveGameUnits = game->getSavedGame()->getSavedBattle()->getUnits();
			for (OpenXcom::BattleUnit* unit : *saveGameUnits)
			{
				if (unit == nullptr)
				{
					continue;
				}
				if (unit->getId() == actorId)
				{
					ba.actor = unit;
					break;
				}
			}
		}

		// get weapon from game battlesave based on ID.
		// assuming theres no specific weapon class?
		if (weaponId != -1)
		{
			auto items = game->getSavedGame()->getSavedBattle()->getItems();
			for (OpenXcom::BattleItem* item : *items)
			{
				if (item == nullptr)
				{
					continue;
				}
				if (item->getId() == weaponId)
				{
					ba.weapon = item;
					break;
				}
			}
		}

		// get skill rule from type name
		if (skillRulesType != "")
		{
			ba.skillRules = game->getMod()->getSkill(skillRulesType);
		}

		return ba;
	}

	// unified serialize function
	template<class TArchive>
	void serialize(TArchive& archive)
	{
		archive(time, energy, morale, health, stun, mana, type, actorId, weaponId, skillRulesType, target, waypoints, targeting, value, result, strafe,
			run, sneak, ignoreSpottedEnemies, kneel, diff, autoShotCounter, cameraPosition, desperate, finalFacing, finalAction, number, sprayTargeting,
			relativeOrigin, terrainMeleeTilePart);
	}

};

struct MultiplayerBattleActionVector
{
	std::vector<MultiplayerBattleAction> orderedActions;

	uint64_t rngSeed;

	MultiplayerBattleActionVector() = default;
	MultiplayerBattleActionVector(std::vector<MultiplayerBattleAction>&& inMoveVec, uint64_t seed) : orderedActions{inMoveVec}, rngSeed{seed} {}

	template<class TArchive>
	void serialize(TArchive& archive)
	{
		archive(orderedActions, rngSeed);
	}
};
