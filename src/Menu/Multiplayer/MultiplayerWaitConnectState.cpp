#include "MultiplayerWaitConnectState.h"

#include <functional>

#include "../../Interface/Text.h"
#include "../../Interface/Window.h"
#include "../../Engine/Game.h"
#include "../../Network/NetHost/XcomNetClient.h"
#include "../../Network/NetHost/XcomNetServer.h"
#include "../../Network/NetHost/XcomNetHost.h"
#include "../../Engine/Logger.h"

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
		textWaitForJoin->setText((type == EHostType::Host) ? "Waiting for player..." : "Connecting to server...");

		setInterface("multiplayerWaitConnect");

		add(textWaitForJoin, "text", "multiplayerWaitConnect");

		onClientConnectConnection = _game->GetNetHost()->BindToOnConnectEvent(std::bind(&MultiplayerWaitConnectState::OnClientConnect, this));
		Log(LOG_INFO) << "Waiting for player to join.";
	}

	void MultiplayerWaitConnectState::OnClientConnect()
	{
		if (hostType == EHostType::Host)
		{
			Log(LOG_INFO) << "Client connected.";
		}
		else
		{
			Log(LOG_INFO) << "Connected to server";
		}
	}
}


