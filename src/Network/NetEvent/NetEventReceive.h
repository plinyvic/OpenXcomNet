#pragma once

#include "../../Network/NetEvent/NetEventBase.h"

#include <cstdint>
#include <memory>

enum class ENetEventReceiveDataType : uint8_t
{
	NONE,
	ERROR_INVALID_BYTES,
	ChatMessage
};

template <typename TDataType, typename TNetHostType>
class TNetEventReceive : public NetEventBase<TNetHostType>
{
  public:
	TDataType eventData;

	TNetEventReceive(const TDataType& inEventData) : eventData(inEventData) {}
};

template <typename TDataType, typename TNetHostType, void (TNetHostType::*TEventFunction)(TNetEventReceive<TDataType, TNetHostType>&)>
class TNetEventReceiveFunc : public TNetEventReceive<TDataType, TNetHostType>
{
  public:
	virtual void HandleEvent(TNetHostType& host) override
	{
		(host.*TEventFunction)(*this);
	}

	TNetEventReceiveFunc(const TDataType& inEventData) : TNetEventReceive<TDataType, TNetHostType>(inEventData) {}
};
