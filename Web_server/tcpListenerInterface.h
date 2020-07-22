#pragma once
#include <WS2tcpip.h> //contains winsock and lots of other helpful stuff


#pragma comment (lib, "ws2_32.lib")
class tcpListenerInterface
{
public:
	//split stuff into init and run
	int init_listener();
	int run_listener();

	tcpListenerInterface(const char* ipaddress, int port) : m_ipaddress(ipaddress), m_port(port) {};
protected:
	virtual void onClientConnect(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

	void sendToClient(int clientSocket, const char* msg, int length);

	void broadcastToClients(int sendingClient, const char* msg, int length);

private:
	const char* m_ipaddress;
	int			m_port;
	int			m_socket;
	fd_set		m_master;//master list of sockets
};

