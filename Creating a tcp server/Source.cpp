#include <iostream>
#include<string>
#include <WS2tcpip.h> //contains winsock and lots of other helpful stuff

#pragma comment (lib, "ws2_32.lib")

int main()
{
	//init winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		std::cerr << "stuff done goofed up" << std::endl;
		return 1;
	}

	//create socket
	//the sock stream is a tcp socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "socket done goofed" << std::endl;
		return 2;
	}
	std::cout << "Listening socket made connection\n";
	//a socket is a number that links the ip address to a specific socket. like port 352

	//bind ipaddress and port to socket
	sockaddr_in hint; //struct that specifies the transport address and port for af_inet address family
	hint.sin_family = AF_INET; //af_inet is the addressfamily for ipv4
	hint.sin_port = htons(54000); //port number
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //could also use inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));//socket, bound to the networking code.

	//tell winsock the socket is for listening
	listen(listening, SOMAXCONN);//number of maximum connections we can have, handled by winsock

	//wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	//so we listened for a connection, that connection has happened on port 54000 and now we can use a different socket to transmit info with a different port number than 54000
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cerr << "client socket done goofed" << std::endl;
		return 3;
	}
	std::cout << "client socket made connection\n";
	char host[NI_MAXHOST]; //clients remote name
	char service[NI_MAXSERV]; //service (i.e. port) the client is connected on

	ZeroMemory(host, NI_MAXHOST);  //same as memset(host, 0 , NI_MAXHOST)
	ZeroMemory(service, NI_MAXSERV);

	//check to get name info, if it doesnt work, we get the ip address
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		//dnss lookup of the info that we have put into host and service
		std::cout << host << " made a love connection to port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " made a love connection on port " << ntohs(client.sin_port) << std::endl; //ntohs = network to host short
	}
	
	//close listening socket
	closesocket(listening);
	std::cout << "closed listening socket\n";
	//while loop: accept and echo message back to client
	char buf[4096]; //should actually be reading until you get no bytes, doing it dynamically, but for this its fine because it wont be above this size

	while (1)
	{
		ZeroMemory(buf, 4096);
		//wait for client to send data
		//use receve function which returns the bytes received, if its an error, something goofed, if its zero the client disconnected
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "We didnt receive shit. Rage Quiting." << std::endl;
			break;
		}
		if (bytesReceived == 0)
		{
			std::cerr << "Client closed. it doesnt liek your server" << std::endl;
			break;
		}

		std::cout << std::string(buf, 0, bytesReceived) << std::endl; //echoing the stuff the client sent to the server side

		//echo message back to client
		int bytesSent = send(clientSocket, buf, bytesReceived + 1, 0); //send returns the amount of bytes sent, bytesReceived+1 so that it sends the 0 flag to signify the end of the data
		if (bytesSent == SOCKET_ERROR)
		{
			std::cerr << "We didnt send shit. Rage Quiting." << std::endl;
			break;
		}
		if (bytesSent == 0)
		{
			std::cerr << "We didnt send any data" << std::endl;
			break;
		}
	}
	//close socket
	closesocket(clientSocket);
	//cleanup winsock
	WSACleanup();
}