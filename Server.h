// Server.h
#include <string.h>
#include <iostream>

const int MAX_CONNECTIONS = 1;

namespace server {
	class Server {
	public:
		Server(int port);
		virtual ~Server() {};
		const Server& operator << (const std::string&) const;
		const Server& operator << (const double) const;
	private:
		int serverSocket;
	};

	class ServerException {
	public:
		ServerException(std::string message) : msg(message) {};
		~ServerException() {};
		std::string description() { return msg; }
	private:
		std::string msg;
	};
}

