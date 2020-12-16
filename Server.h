// Server.h
#include <string.h>
#include <iostream>
#include <set>

// Include websocketpp
// The ASIO_STANDALONE define is necessary to use the standalone version of Asio.
#define ASIO_STANDALONE
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

const int MAX_CONNECTIONS = 1;

namespace server {
	typedef websocketpp::server<websocketpp::config::asio> sock_serv;

	class Server {
	public:
		Server();
		bool run(int port);
		virtual ~Server() {};
		const Server& operator << (const std::string&);
	private:
		typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_list;

		void onOpen(websocketpp::connection_hdl hdl);
		void onClose(websocketpp::connection_hdl hdl);
		void onFail(websocketpp::connection_hdl hdl);

		con_list connections;
		sock_serv endpoint;
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
