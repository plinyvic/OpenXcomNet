#pragma once
#include "../../Engine/State.h"
#include <boost/signals2.hpp>

namespace OpenXcom
{
	class Text;
class Window;
	enum class EHostType
	{
		Host,
		Client
	};

	class MultiplayerWaitConnectState : public State
	{
	private:

		

		EHostType hostType;

		Text* textWaitForJoin;

		std::string connectString;

		boost::signals2::scoped_connection onClientConnectConnection;
		boost::signals2::scoped_connection onReceiveHostSave;
		boost::signals2::scoped_connection onReceiveClientReady;
		boost::signals2::scoped_connection onReceiveStartMatch;

	public:

		MultiplayerWaitConnectState(EHostType type, std::string connectString);

		void OnConnect();

		void OnReceiveHostSave();

		void OnReceiveClientReady();

		void OnReceiveStartMatch();
	};
}
