#pragma once
#include "NetHost.h"
#include <boost/signals2.hpp>

enum class EXcomNetEventType : uint8_t
{
	NET_ERROR,
	ReceiveHostSave,
	ReceiveClientReady,
	ReceiveStartMatch,
	PushBattleActionFront,
	PushBattleActionNext,
	PushBattleActionBack
};

namespace OpenXcom
{
	class BattleState;
	class Game;
}

struct MultiplayerBattleAction;

class XcomNetHost : public NetHost
{
public:

	virtual ~XcomNetHost() override = default;

protected:

	virtual void HandleReceiveEvent(ENetEvent& event) override;

protected:

	OpenXcom::Game* game;

private:

	boost::signals2::signal<void(std::string&)> receiveHostSaveSignal;
	boost::signals2::signal<void()>	receiveClientReadySignal;
	boost::signals2::signal<void()> receiveStartMatchSignal;

	boost::signals2::signal<void(MultiplayerBattleAction&)> receivePushBattleActionFrontSignal;
	boost::signals2::signal<void(MultiplayerBattleAction&)> receivePushBattleActionNextSignal;
	boost::signals2::signal<void(MultiplayerBattleAction&)> receivePushBattleActionBack;

private:

	OpenXcom::BattleState* MakeBattleState(MultiplayerBattleAction& battleAction); 

public:

	void SetGame(OpenXcom::Game* inGame);

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, std::string&>
	inline boost::signals2::connection BindToReceiveHostSaveEvent(TCallable&& function)
	{
		return receiveHostSaveSignal.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void>
	inline boost::signals2::connection BindToReceiveClientReadyEvent(TCallable&& function)
	{
		return receiveClientReadySignal.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void>
	inline boost::signals2::connection BindToReceiveStartMatchEvent(TCallable&& function)
	{
		return receiveStartMatchSignal.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, MultiplayerBattleAction&>
	inline boost::signals2::connection BindToReceivePushBattleActionFrontEvent(TCallable&& function)
	{
		return receivePushBattleActionFrontSignal.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, MultiplayerBattleAction&>
	inline boost::signals2::connection BindToReceivePushBattleActionNextEvent(TCallable&& function)
	{
		return receivePushBattleActionNextSignal.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, MultiplayerBattleAction&>
	inline boost::signals2::connection BindToReceivePushBattleActionBackEvent(TCallable&& function)
	{
		return receivePushBattleActionBack.connect(std::forward<TCallable>(function));
	}

};
