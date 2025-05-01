#include "MultiplayerStartState.h"
#include "../../Interface/Window.h"
#include "../../Interface/TextButton.h"
#include "../../Interface/TextEdit.h"
#include "../../Engine/Game.h"
#include "../../Engine/Options.h"
#include "MultiplayerNewBattleState.h"
#include "MultiplayerWaitConnectState.h"

OpenXcom::MultiplayerStartState::MultiplayerStartState() : ipAddress{"127.0.0.1"}
{
	window = new Window(this, 256, 160, 32, 20, POPUP_NONE);

	buttonHost = new TextButton(92, 20, 64, 90);
	buttonJoin = new TextButton(92, 20, 164, 90);
	buttonCancel = new TextButton(92, 20, 164, 146);

	textEditIpAddress = new TextEdit(this, 92, 20, 114, 62);

	setInterface("multiplayerMenu");

	add(window, "window", "multiplayerMenu");
	add(buttonHost, "button", "multiplayerMenu");
	add(buttonJoin, "button", "multiplayerMenu");
	add(buttonCancel, "button", "multiplayerMenu");
	add(textEditIpAddress, "ipAddress", "multiplayerMenu");

	setWindowBackground(window, "mainMenu");

	buttonHost->onMouseClick((ActionHandler)&MultiplayerStartState::ButtonHostHandler);
	buttonHost->setText("Host");

	buttonJoin->onMouseClick((ActionHandler)&MultiplayerStartState::ButtonJoinHandler);
	buttonJoin->setText("Join");

	buttonCancel->onMouseClick((ActionHandler)&MultiplayerStartState::ButtonCancelHandler);
	buttonCancel->onKeyboardPress((ActionHandler)&MultiplayerStartState::ButtonCancelHandler, Options::keyCancel);
	buttonCancel->setText("Cancel");

	textEditIpAddress->setAlign(ALIGN_CENTER);
	textEditIpAddress->setBig();
	textEditIpAddress->setConstraint(TEC_NONE);
	textEditIpAddress->setText(ipAddress);
	textEditIpAddress->onChange((ActionHandler)&MultiplayerStartState::TextEditIpAddressHandler);
}

void OpenXcom::MultiplayerStartState::ButtonHostHandler(Action* action)
{
	_game->pushState(new MultiplayerNewBattleState());
}

void OpenXcom::MultiplayerStartState::ButtonJoinHandler(Action* action)
{
	_game->pushState(new MultiplayerWaitConnectState(EHostType::Client, ipAddress));
}

void OpenXcom::MultiplayerStartState::ButtonCancelHandler(Action* action)
{
	_game->popState();
}

void OpenXcom::MultiplayerStartState::TextEditIpAddressHandler(Action* action)
{
	ipAddress = textEditIpAddress->getText();
}
