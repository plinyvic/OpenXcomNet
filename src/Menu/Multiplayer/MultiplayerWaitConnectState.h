#pragma once
#include "../../Engine/State.h"
#include <boost/signals2.hpp>

struct ConnectEvent;

namespace OpenXcom
{
	class Text;
	class Window;
	class Craft;
	enum class EHostType
	{
		Host,
		Client
	};

	class MultiplayerWaitConnectState : public State
	{
	private:

		Craft* _craft;
		EHostType hostType;

		Text* textWaitForJoin;

		std::string connectString;

		std::string battleState;

		boost::signals2::scoped_connection onClientConnectConnection;
		boost::signals2::scoped_connection onReceiveHostSave;
		boost::signals2::scoped_connection onReceiveClientReady;
		boost::signals2::scoped_connection onReceiveStartMatch;

	public:

		MultiplayerWaitConnectState(EHostType type, std::string connectString);
		MultiplayerWaitConnectState(EHostType type, std::string connectString, std::string& battleStateString);

		void OnConnect(ConnectEvent& event);

		void OnReceiveHostSave(std::string& saveString);

		void OnReceiveClientReady();

		void OnReceiveStartMatch();

		void StartMatch();
	};
}
