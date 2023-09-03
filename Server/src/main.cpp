#include <iostream>
#include "msk_net.h"


enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomServer : public net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort)
		: net::server_interface<CustomMsgTypes>(nPort)
	{

	}
protected:
	virtual bool OnClientConnect(std::shared_ptr<net::connection<CustomMsgTypes>> client) override
	{
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	virtual void OnClientValidated(std::shared_ptr<net::connection<CustomMsgTypes>> client) override
	{
		net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
	}

	virtual void OnClientDisconnect(std::shared_ptr<net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	virtual void OnMessage(std::shared_ptr<net::connection<CustomMsgTypes>> client, net::message<CustomMsgTypes>& msg) override
	{
		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerPing:
			std::cout << "[" << client->GetID() << "]: Server Ping\n";
			client->Send(msg);
			break;
		case CustomMsgTypes::MessageAll:
			std::cout << "[" << client->GetID() << "]: Message All\n";
			//net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ServerMessage;
			//msg << client->GetID();
			//msg << (uint32_t)'a';
			//msg << (uint32_t)'b';
			//msg << (uint32_t)'c';
			//msg << (uint32_t)3; // size of sentence
			MessageAllClients(msg, client);
			break;
		}
		
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}