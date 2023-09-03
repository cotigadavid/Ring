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
		msg << "faf";

		Send(msg);
	}
	void MessageAll(const char* s)
	{
		net::message<CustomMsgTypes> msg;

		msg.header.id = CustomMsgTypes::MessageAll;
		
		msg << (uint32_t)m_connection->GetID();
		
		std::cout << m_connection->GetID();

		for (int i = 0; i < strlen(s); ++i)
			msg << s[i];
		msg << (uint32_t)strlen(s);

		Send(msg);
	}
};
