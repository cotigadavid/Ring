#pragma once

#include "msk_net.h"

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomClient : public net::client_interface<CustomMsgTypes>
{
public:
	void PingServer()
	{
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		msg << timeNow;
		Send(msg);
	}

	void MessageAll()
	{
		net::message<CustomMsgTypes> msg;

		msg.header.id = CustomMsgTypes::MessageAll;
		msg << "DEFAULT MESSAGE ALL FUNCTION";

		Send(msg);
	}
	void MessageAll(const char* name, const char* message)
	{
		net::message<CustomMsgTypes> msg;

		msg.header.id = CustomMsgTypes::MessageAll;
		
		msg << (uint32_t)m_connection->GetID();
		
		for (int i = 0; i < strlen(name); ++i)
			msg << name[i];
		msg << (uint32_t)strlen(name);

		for (int i = 0; i < strlen(message); ++i)
			msg << message[i];
		msg << (uint32_t)strlen(message);

		Send(msg);
	}
};
