#include <iostream>
#include <fstream>
#include "msk_net.h"

std::ofstream fout;

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

		std::ifstream fin;
		fin.open("../../../SavedConv.txt");

		std::string s;
		std::string name;
		std::string message;

		while (getline(fin, s))
		{
			net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::ServerMessage;
			unsigned int pos = s.find(":");
			name = s.substr(0, pos);
			message = s.substr(pos + 2, s.size() - pos);

			msg2 << (uint32_t)client->GetID();
			for (int i = 0; i < name.size(); ++i)
				msg2 << name[i];
			msg2 << (uint32_t)name.size();

			for (int i = 0; i < message.size(); ++i)
				msg2 << message[i];
			msg2 << (uint32_t)message.size();

			client->Send(msg2);
		}
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
			msg.header.id = CustomMsgTypes::ServerMessage;
			
			net::message<CustomMsgTypes> copyMsg = msg;

			uint32_t clientID;
			uint32_t sizeOfSentence;
			uint32_t sizeOfName;
			uint8_t indChar;
			std::string receivedMessage;
			std::string receivedName;

			copyMsg >> sizeOfSentence;

			while (sizeOfSentence)
			{
				sizeOfSentence--;
				copyMsg >> indChar;
				receivedMessage += indChar;
			}

			copyMsg >> sizeOfName;

			while (sizeOfName)
			{
				sizeOfName--;
				copyMsg >> indChar;
				receivedName += indChar;
			}

			std::reverse(receivedMessage.begin(), receivedMessage.end());
			std::reverse(receivedName.begin(), receivedName.end());

			copyMsg >> clientID;
			
			fout.open("../../../SavedConv.txt", std::ofstream::app);
			fout << receivedName << ": " << receivedMessage << "\n";

			fout.close();

			MessageAllClients(msg, client);
			break;
		}
		
	}
};

int main()
{
	int portNr;

	std::cin >> portNr;

	CustomServer server(portNr);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}