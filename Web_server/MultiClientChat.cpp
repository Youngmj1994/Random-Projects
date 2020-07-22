#include "MultiClientChat.h"

void MultiClientChat::onClientConnect(int clientSocket)
{
	std::string welcomemsg = "Hello, welcome to Josh's Server!";
	sendToClient(clientSocket, welcomemsg.c_str(), welcomemsg.size() + 1);
}

void MultiClientChat::onClientDisconnected(int clientSocket)
{
}

void MultiClientChat::onMessageReceived(int clientSocket, const char* msg, int length)
{
	broadcastToClients(clientSocket, msg, length);
}
