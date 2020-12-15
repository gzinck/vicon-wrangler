#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h> 

using namespace server;

Server::Server(int port) {
	int serverFD;
	sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1;

	// Get a file descriptor for the socket
	if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		std::cerr << "Server could not create socket file descriptor" << std::endl;
		throw ServerException("Server could not create socket file descriptor");
	}

	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		std::cerr << "Server could not set the socket options" << std::endl;
		throw ServerException("Server could not set the socket options");
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(serverFD, (struct sockaddr*) &address, sizeof(address)) < 0) {
		std::cerr << "Server could not bind the socket" << std::endl;
		throw ServerException("Server could not bind the socket");
	}

	if (listen(serverFD, MAX_CONNECTIONS) < 0) {
		std::cerr << "Server could not listen on socket" << std::endl;
		throw ServerException("Server could not listen on socket");
	}

	if ((serverSocket = accept(serverFD, (struct sockaddr*) &address, (socklen_t*)&addrlen)) < 0) {
		std::cerr << "Server could not accept connections on the socket" << std::endl;
		throw ServerException("Server could not accept connections on the socket");
	}
}

const Server& Server::operator << (const std::string& s) const {
	if (send(serverSocket, s.c_str(), s.size(), 0) < 0) {
		std::cerr << "Server could not send message" << std::endl;
		throw ServerException("Server could not send message");
	}
	return *this;
}

const Server& Server::operator << (const double num) const {
	const std::string s = std::to_string(num);
	if (send(serverSocket, s.c_str(), s.size(), 0) < 0) {
		std::cerr << "Server could not send message" << std::endl;
		throw ServerException("Server could not send message");
	}
	return *this;
}
