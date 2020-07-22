#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main()
{
	string ipAddressOfServer = "10.21.3.247";
	int listeningPort = 54000;

	//init winscock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsReseult = WSAStartup(ver, &data);
	if (wsReseult != 0)
	{
		cerr << "winsock messed up, error number = " << wsReseult << endl;
		return wsReseult;
	}

	//create socket, identifier that communicates with network layer that sends data out ot the remote machine, similar to server side, but now with the client
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		
		cerr << "cant make socket, error number = " << WSAGetLastError() << endl;
	} 

	//fill in hint structure, tell winsock what port and what server we want to connect to
	sockaddr_in hint; //ipv4 version of the structure is why its _in
	hint.sin_family = AF_INET;
	hint.sin_port = htons(listeningPort);
	inet_pton(AF_INET, ipAddressOfServer.c_str(), &hint.sin_addr); //similar to binding from the server. but not the same.

	//connect to a server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "cant connect to server, error number = " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		system("PAUSE");
		return -1;
	}

	//do_while loop to send and receive data
	char buf[4096];
	string userInput;
	do
	{
		//prompt user for text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) //make sure user typed something
		{	
			//send text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				//wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					//echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
				else
				{
					cerr << "Send failed" << endl;
					return -2;
				}
			}
		}
		
		
	} while (userInput.size() > 0) ; 

	//close everything
	closesocket(sock);
	WSACleanup();
	return 0;
}