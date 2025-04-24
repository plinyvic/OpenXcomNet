#pragma once

#include "../Engine/State.h"
namespace OpenXcom
{

	class TextButton;
	class Window;
	class Text;
	class TextEdit;

class MultiplayerStartState : public State
{
private:

	TextButton*	buttonHost;
	TextButton*	buttonJoin;
	TextButton* buttonCancel;
	TextEdit*	textEditIpAddress;
	Window*		window;

	std::string ipAddress;

public:

	MultiplayerStartState();

	~MultiplayerStartState() = default;

	void ButtonHostHandler(Action* action);
	void ButtonJoinHandler(Action* action);
	void ButtonCancelHandler(Action* action);
	void TextEditIpAddressHandler(Action* action);

};
}
