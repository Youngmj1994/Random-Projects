#pragma once
#include "tcpListenerInterface.h"
#include<string>

class MultiClientChat : public tcpListenerInterface
{
public:
	MultiClientChat(const char* ipAddress, int port) : tcpListenerInterface(ipAddress, port) {}

protected:
	void onClientConnect(int clientSocket) override;

	void onClientDisconnected(int clientSocket) override;

	void onMessageReceived(int clientSocket, const char* msg, int length) override;
};

