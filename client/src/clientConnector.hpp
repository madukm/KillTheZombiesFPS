#ifndef CLIENT_CONNECTOR_H
#define CLIENT_CONNECTOR_H
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "../../shared/logger.hpp"
#include "objects/survivor.hpp"

class ClientConnector
{
	public:
		ClientConnector();
		~ClientConnector();

		void connectToServer();
		void updateServerState(std::vector<Survivor*>& survivors);

	private:
		int _sock;
		bool _connected;
		int _clientId;
		struct sockaddr_in _serverAddr;

		Logger* _log;
};

#endif// CLIENT_CONNECTOR_H
