#pragma once
#include "../BattlescapeGame.h"

#include "../../Mod/RuleItem.h"
#include "../../Mod/RuleSkill.h"
#include "../../Savegame/BattleItem.h"
#include <cereal/types/string.hpp>
#include <cereal/types/list.hpp>
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

// Battle action with extra properties for use in multiplayer.
// Also contains save/load for networking.
struct MultiplayerBattleAction : OpenXcom::BattleAction
{
	// dont serialize this
	OpenXcom::Game* game;

	uint64_t rngSeed;

	// rng seed goes here <3

	// default, probably not used?
	MultiplayerBattleAction() : rngSeed{0}, game{nullptr} {}
	// incoming action constructor
	MultiplayerBattleAction(OpenXcom::Game* inGame) : BattleAction(), game{inGame} {}

	// outgoing action constructor
	MultiplayerBattleAction(OpenXcom::Game* inGame, uint64_t inSeed, const BattleAction& inAction) : BattleAction{inAction}, game{inGame}, rngSeed{inSeed} {}

	// cereal save/load interface
	template <class TArchive>
	void save(TArchive& archive) const
	{
		// archive RuleItemUseRuleBase<uint_16> properties
		archive(Time, Energy, Morale, Health, Stun, Mana);

		// archive BattleActionCost properties
		archive(type);
		// check pointer types. if they are valid, serialize their uniquely identifying property.
		if (actor != nullptr)
		{
			// id is unique
			archive(actor->getId());
		}
		else
		{
			archive(int{-1});
		}
		if (weapon != nullptr)
		{
			// id is unique
			archive(weapon->getId());
		}
		else
		{
			archive(int{-1});
		}
		if (skillRules != nullptr)
		{
			// type string should be unique
			archive(skillRules->getType());
		}
		else
		{
			archive(std::string{"nullptr"});
		}

		// serialize BattleAction properties
		archive(target, waypoints, targeting, value, result, strafe, run, sneak, ignoreSpottedEnemies, kneel, diff, autoShotCounter,
				cameraPosition, desperate, finalFacing, finalAction, number, sprayTargeting, relativeOrigin, terrainMeleeTilePart);

		// serialize rngSeed
		archive(rngSeed);
	}

	template <class TArchive>
	void load(TArchive& archive)
	{
		// archive RuleItemUseRuleBase<uint_16> properties
		archive(Time, Energy, Morale, Health, Stun, Mana);

		// archive BattleActionCost properties
		archive(type);

		// get unit from game battlesave based on ID.
		int baActorId;
		archive(baActorId);
		actor = nullptr;
		if (baActorId != -1)
		{
			std::vector<OpenXcom::BattleUnit*>* saveGameUnits = game->getSavedGame()->getSavedBattle()->getUnits();
			for (OpenXcom::BattleUnit* unit : *saveGameUnits)
			{
				if (unit == nullptr)
				{
					continue;
				}
				if (unit->getId() == baActorId)
				{
					actor = unit;
					break;
				}
			}
		}

		// get weapon from game battlesave based on ID.
		// assuming theres no specific weapon class?
		int baWeaponId;
		archive(baWeaponId);
		weapon = nullptr;
		if (baWeaponId != -1)
		{
			auto items = game->getSavedGame()->getSavedBattle()->getItems();
			for (OpenXcom::BattleItem* item : *items)
			{
				if (item == nullptr)
				{
					continue;
				}
				if (item->getId() == baWeaponId)
				{
					weapon = item;
					break;
				}
			}
		}

		// get skill rule from type name
		std::string baRuleSkillType;
		archive(baRuleSkillType);
		skillRules = nullptr;
		if (baRuleSkillType != "nullptr")
		{
			skillRules = game->getMod()->getSkill(baRuleSkillType);
		}

		// deserialize BattleAction properties
		archive(target, waypoints, targeting, value, result, strafe, run, sneak, ignoreSpottedEnemies, kneel, diff, autoShotCounter,
				cameraPosition, desperate, finalFacing, finalAction, number, sprayTargeting, relativeOrigin, terrainMeleeTilePart);

		// deserialize rngSeed
		archive(rngSeed);
	}
};
