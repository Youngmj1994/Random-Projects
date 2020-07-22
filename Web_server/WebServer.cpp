#include "WebServer.h"


void WebServer::onClientConnect(int clientSocket)
{

}
void WebServer::onClientDisconnected(int clientSocket)
{
}

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length)
{
//get /index.html http/1.1

	//parse the document
	std::istringstream iss(msg);
	std::vector<std::string> parsed_results((std::istream_iterator<std::string>(iss)),std::istream_iterator<std::string>());

	//open document in local system
	
	std::string content = "<h1>404 Not Found</h1>";
	std::string htmlFile = "/index.html";
	int errorCode = 404;

	// If the GET request is valid, try and get the name
	if (parsed_results.size() >= 3 && parsed_results[0] == "GET")
	{
		htmlFile = parsed_results[1];

		// If the file is just a slash, use index.html. This should really
		// be if it _ends_ in a slash. I'll leave that for you :)
		if (htmlFile == "/")
		{
			htmlFile = "/index.html";
		}
	}

	// Open the document in the local file system
	std::ifstream f(".\\wwwroot" + htmlFile);

	// Check if it opened and if it did, grab the entire contents
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		content = str;
		errorCode = 200;
	}
	
	f.close();

	std::ostringstream oss;
	oss << "HTTP/1.1 " << errorCode << " OK\r\n";
	oss << "Cache-Control: no-chache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << content.size() << "\r\n";
	oss << "\r\n";
	oss << content;

	std::string output = oss.str();
	int size = output.size() + 1;

	sendToClient(clientSocket, output.c_str(), size);
}

