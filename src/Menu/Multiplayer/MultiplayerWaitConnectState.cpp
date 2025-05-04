#include "MultiplayerWaitConnectState.h"

#include <functional>

#include "../../Interface/Text.h"
#include "../../Interface/Window.h"
#include "../../Engine/Game.h"
#include "../../Network/NetHost/XcomNetClient.h"
#include "../../Network/NetHost/XcomNetServer.h"
#include "../../Network/NetHost/XcomNetHost.h"
#include "../../Engine/Logger.h"
#include "../../Savegame/SavedGame.h"
#include "../../Battlescape/BriefingState.h"
#include <cereal/types/string.hpp>
#include "../../Battlescape/BattlescapeGenerator.h"
#include "MultiplayerNewBattleState.h"
#include "../../Savegame/Base.h"
#include "../../Savegame/Craft.h"
#include "../../Savegame/SavedBattleGame.h"

namespace OpenXcom
{
MultiplayerWaitConnectState::MultiplayerWaitConnectState(EHostType type, std::string connectString) : hostType{type}, connectString{connectString}, textWaitForJoin{new Text(320, 17, 0, 92)}
	{
		if (type == EHostType::Host)
		{
			_game->SetNetworkControllerHost(std::move(std::make_unique<XcomNetServer>()));
		}
		else
		{
			_game->SetNetworkControllerHost(std::move(std::make_unique<XcomNetClient>(connectString)));
		}

		// TODO: move to ctor
		_game->GetNetHost()->SetGame(_game);
		textWaitForJoin->setText((type == EHostType::Host) ? "Waiting for player..." : "Connecting to server...");

		setInterface("multiplayerWaitConnect");

		add(textWaitForJoin, "text", "multiplayerWaitConnect");

		onClientConnectConnection = _game->GetNetHost()->BindToOnConnectEvent([this](ConnectEvent& event)
																			  {OnConnect(event);});

		
		Log(LOG_INFO) << "Waiting for player to join.";
	}

	MultiplayerWaitConnectState::MultiplayerWaitConnectState(EHostType type, std::string connectString, std::string& battleStateString) : MultiplayerWaitConnectState(type, connectString)
	{
		battleState = battleStateString;
	}

	void MultiplayerWaitConnectState::OnConnect(ConnectEvent& event)
	{
		if (hostType == EHostType::Host)
		{
			Log(LOG_INFO) << "Client connected.";
			// for host, send packet containing save to client.
			std::string saveString = _game->getSavedGame()->GetFinalSaveString(_game->getMod());

			_game->GetNetworkControllerMutable().CreateOutboundPacket(EXcomNetEventType::ReceiveHostSave, saveString);
			// wait for client ready
			onReceiveClientReady = _game->GetNetHost()->BindToReceiveClientReadyEvent([this](){ OnReceiveClientReady(); });
		}
		else if (hostType == EHostType::Client)
		{
			Log(LOG_INFO) << "Connected to server";
			// for client, wait for host to send battlescape save
			onReceiveHostSave = _game->GetNetHost()->BindToReceiveHostSaveEvent([this](std::string& s)
																				{ OnReceiveHostSave(s); });
		}
		else
		{
			// something has gone horribly wrong
		}
	}

	void MultiplayerWaitConnectState::OnReceiveHostSave(std::string& saveString)
	{
		// trash current save and load network save
		auto networkSavedGame = new SavedGame();
		networkSavedGame->LoadFromString(saveString, _game->getMod(), _game->getLanguage());
		_game->setSavedGame(networkSavedGame);
		networkSavedGame->getSavedBattle()->loadMapResources(_game->getMod());

		// tell host that client is ready to start
		_game->GetNetworkControllerMutable().CreateOutboundPacketVoid(EXcomNetEventType::ReceiveClientReady);

		// wait for host to start game
		onReceiveStartMatch = _game->GetNetHost()->BindToReceiveStartMatchEvent([this]()
																				{ OnReceiveStartMatch(); });
	}

	void MultiplayerWaitConnectState::OnReceiveClientReady()
	{
		// tell client we are starting match.
		_game->GetNetworkControllerMutable().CreateOutboundPacketVoid(EXcomNetEventType::ReceiveStartMatch);
		// ...then start the match
		StartMatch();
	}

	void MultiplayerWaitConnectState::OnReceiveStartMatch()
	{
		StartMatch();
	}

	void MultiplayerWaitConnectState::StartMatch()
	{
		if (hostType == EHostType::Client)
		{
			
		}
		else
		{
			
		}
		Craft* craft = (*(*_game->getSavedGame()->getBases())[0]->getCrafts())[0];
		_game->pushState(new BriefingState(craft, craft->getBase()));
	}
}


