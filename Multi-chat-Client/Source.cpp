/*
make a chat app, where you can have multiple clients and one server
we are going to do this using the select() function. 
its got 
FD_CLR remove from set
FD_SET add to set
FD_ZERO clear the whole set
fd_set which has an array, which has the number of sockets we are interested in, could be listenner and 3 clients, etc.
this could be done using multiple threads but frankly thats a nightmare

*/


#include <iostream>
#include<string>
#include<sstream>
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
//========above is regular tcp server code, below is new=======================================================//

	fd_set master; //init the array of sockets
	FD_ZERO(&master); //zeroing everything

	FD_SET(listening, &master); //adding the listening socket to the master list

	while (1)
	{
		fd_set copy = master;//do this to avoid destroying everything in the master

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr); //read,write,exeptions,timeout. this is basically making a queue, so you will have a backlog of clients to deal with and some will have to wait
		for (int i = 0; i < socketCount; i++)
		{
			//the two things a server can do is request info from a client and receive info from a client
			//if the socket at position i is the listening socket thats an inbound communication
			//otherwise its a message from the client thats connected
			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				//accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr); //can put in more info about the client

				//add the new connection to the fd_array
				FD_SET(client, &master);
				//send welcome message to the connected client
				std::string welcomeMSG = "welcome to chat you jerk\r\n";
				send(client, welcomeMSG.c_str(), welcomeMSG.size() + 1, 0);

				//TODO: broadcast to all clients that someone has joined
				//for loop through the array to clients that the new person joined
			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				//receive message
				int bytesIn = recv(sock,buf,4096,0);
				if (bytesIn <= 0)
				{
					//drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					//send message to other clients, NOT the listening socket
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							//can add a vector, so that you can change peoples names
							std::ostringstream ss;
							ss << "Socket #" << sock << ": " << buf << "\r\n";
							std::string strOut = ss.str();

							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
				}

			}
		}

	}


	WSACleanup();
	return 0;
}