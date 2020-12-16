#include "Server.h"

using namespace server;

void Server::onOpen(websocketpp::connection_hdl hdl) {
	connections.insert(hdl);
	// endpoint.send(hdl, "Connection successful!", websocketpp::frame::opcode::text);
	std::cout << "Connection opened" << std::endl;
}

void Server::onClose(websocketpp::connection_hdl hdl) {
	connections.erase(hdl);
	std::cout << "Connection closed" << std::endl;
}

void Server::onFail(websocketpp::connection_hdl hdl) {
	connections.erase(hdl);
	std::cout << "Connection failed and closed" << std::endl;
}

Server::Server() {
	// Set up logging
	endpoint.set_error_channels(websocketpp::log::elevel::all);
	endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
	
	// Initialize
	endpoint.init_asio();

	using websocketpp::lib::placeholders::_1;
	using websocketpp::lib::bind;
	endpoint.set_open_handler(bind(&Server::onOpen, this, _1));
	endpoint.set_close_handler(bind(&Server::onClose, this, _1));
	endpoint.set_fail_handler(bind(&Server::onFail, this, _1));
}

bool Server::run(int port) {
	try {
		endpoint.listen(port);
	} catch (websocketpp::exception const &e) {
		std::cout << "Error listening on port " << port << ": " << e.what() << std::endl;
		return false;
	}

	websocketpp::lib::error_code ec;
	endpoint.start_accept();
	if (ec) {
		std::cout << "Error starting to accept connections: " << ec.message() << std::endl;
		return false;
	}

	try {
		endpoint.run();
	} catch (websocketpp::exception const &e) {
		std::cout << "Error running server: " << e.what() << std::endl;
		return false;
	}

	return true;
}

const Server& Server::operator << (const std::string& s) {
	for (websocketpp::connection_hdl hdl : connections) {
		endpoint.send(hdl, s, websocketpp::frame::opcode::text);
	}

	return *this;
}
