#include "tcpListenerInterface.h"

int tcpListenerInterface::init_listener()
{
	//init winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		
		return wsOK;
	}

	//create socket
	//the sock stream is a tcp socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		
		return WSAGetLastError();
	}

	sockaddr_in hint; //struct that specifies the transport address and port for af_inet address family
	hint.sin_family = AF_INET; //af_inet is the addressfamily for ipv4
	hint.sin_port = htons(m_port); //port number
	inet_pton(AF_INET,m_ipaddress,&hint.sin_addr); //pointer to a string to a number

	if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)//socket, bound to the networking code.
		return WSAGetLastError();

	//tell winsock the socket is for listening
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)//number of maximum connections we can have, handled by winsock
		return WSAGetLastError();

	FD_ZERO(&m_master);
	FD_SET(m_socket, &m_master);

	return 0;
}

int tcpListenerInterface::run_listener()
{
	
	bool running = true;
	while (running)
	{
		fd_set copy = m_master;//do this to avoid destroying everything in the master

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr); //read,write,exeptions,timeout. this is basically making a queue, so you will have a backlog of clients to deal with and some will have to wait
		for (int i = 0; i < socketCount; i++)
		{
			//the two things a server can do is request info from a client and receive info from a client
			//if the socket at position i is the listening socket thats an inbound communication
			//otherwise its a message from the client thats connected
			SOCKET sock = copy.fd_array[i];
			if (sock == m_socket)
			{
				//accept a new connection
				SOCKET client = accept(m_socket, nullptr, nullptr); //can put in more info about the client

				//add the new connection to the fd_array
				FD_SET(client, &m_master);

				onClientConnect(client);
				//for loop through the array to clients that the new person joined
			}
			else //its an inbound message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				//receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					onClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &m_master);
				}
				else
				{
					onMessageReceived(sock,buf,bytesIn);
				}

			}
		}

	}
	FD_CLR(m_socket, &m_master);
	closesocket(m_socket);

	while (m_master.fd_count > 0)
	{
		//clean up master
		SOCKET sock = m_master.fd_array[0];
		FD_CLR(sock, &m_master);
		closesocket(sock);
	}

	WSACleanup();
	return 0;
}

void tcpListenerInterface::onClientConnect(int clientSocket)
{
}

void tcpListenerInterface::onClientDisconnected(int clientSocket)
{
}

void tcpListenerInterface::onMessageReceived(int clientSocket, const char* msg, int length)
{
}

void tcpListenerInterface::sendToClient(int clientSocket, const char* msg, int length)
{
	send(clientSocket, msg, length, 0);
}

void tcpListenerInterface::broadcastToClients(int sendingClient, const char* msg, int length)
{
	for (int i = 0; i < m_master.fd_count; i++)
	{
		SOCKET outSock = m_master.fd_array[i];
		if (outSock != m_socket && outSock != sendingClient)
		{
			sendToClient(outSock, msg, length);
		}
	}
}
