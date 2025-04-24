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

template<typename TDataType>
class TNetEventReceive : public NetEventBase
{
public:

	TDataType eventData;

	TNetEventReceive(const TDataType& inEventData) : eventData(inEventData) {}

};

template<typename TDataType, void(NetHost::*TEventFunction)(TNetEventReceive<TDataType>&)>
class TNetEventReceiveFunc : public TNetEventReceive<TDataType>
{
public:

	virtual void HandleEvent(NetHost& host) override
	{
		(host.*TEventFunction)(*this);
	}

	TNetEventReceiveFunc(const TDataType& inEventData) : TNetEventReceive<TDataType>(inEventData) {}

};

template<typename TPtrDataType>
class TNetEventReceive<TPtrDataType*> : public NetEventBase
{
public:

	std::unique_ptr<TPtrDataType> eventData;

	TNetEventReceive(std::unique_ptr<TPtrDataType> inPtr) : eventData(std::move(inPtr)) {}

};
