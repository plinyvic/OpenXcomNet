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
	PushBattleActionBack,
	FlushBattleActions,
	EndTurn
};

namespace OpenXcom
{
	class BattleState;
	class Game;
}

struct MultiplayerBattleActionVector;

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

	boost::signals2::signal<void(MultiplayerBattleActionVector&)> receiveFlushBattleActions;
	boost::signals2::signal<void(uint64_t)> receiveEndTurnSignal;


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
	requires CallableWithSignature<TCallable, void, MultiplayerBattleActionVector&>
	inline boost::signals2::connection BindToReceiveFlushBattleActionsEvent(TCallable&& function)
	{
		return receiveFlushBattleActions.connect(std::forward<TCallable>(function));
	}

	template <typename TCallable>
	requires CallableWithSignature<TCallable, void, uint64_t>
	inline boost::signals2::connection BindToReceiveEndTurnEvent(TCallable&& function)
	{
		return receiveEndTurnSignal.connect(std::forward<TCallable>(function));
	}

};
