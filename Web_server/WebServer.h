#pragma once
#include "tcpListenerInterface.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
class WebServer: public tcpListenerInterface
{ 
	
	public:
		WebServer(const char* ipAddress, int port) : tcpListenerInterface(ipAddress, port) {}

	protected:
		void onClientConnect(int clientSocket) override;

		void onClientDisconnected(int clientSocket) override;

		void onMessageReceived(int clientSocket, const char* msg, int length) override;
	
};

