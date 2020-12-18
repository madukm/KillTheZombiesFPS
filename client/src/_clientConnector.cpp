#include "clientConnector.hpp"
#include <unistd.h>
#include <iostream>

//#include "nlohmann/json.hpp"
//
//using namespace nlohmann;

#define PORT 14230

ClientConnector::ClientConnector():
	_connected(false), _clientId(-1)
{
	_log = new Logger("Client Connector");
}

ClientConnector::~ClientConnector()
{
	if(_log != nullptr)
	{
		delete _log;
		_log = nullptr;
	}
}

void ClientConnector::connectToServer()
{
	// Define socket type (TCP)
	if((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
		_log->log("Socket creation error", DEBUG);
        exit(1);
    }

	// Populate server address
	_serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "18.218.11.188", &_serverAddr.sin_addr)<=0)
    {
		_log->log("Invalid address/ Address not supported.", DEBUG);
		exit(1);
    }

	// Connect to server
	if(connect(_sock, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0) 
	{
		_log->log("Failed to connect to the server!", DEBUG); 
		_connected = false;
	}
	else
	{
		_log->log("Connected to server.", DEBUG);
		_connected = true;
	}
}

void ClientConnector::updateServerState(std::vector<Survivor*>& survivors)
{
	if(_connected)
	{
		//---------- Send survivor data ----------//
		json messageJson = survivors[0]->getJson();
		messageJson["id"] = _clientId;
		std::string message = messageJson.dump();
		send(_sock , message.c_str() , message.size() , 0 ); 
		
		//---------- Receive game state ----------//
		char buffer[10000] = {0};
		int size = read(_sock, buffer, 10000); 

		if(size <= 0)
		{
			_connected = false;
			_log->log("Server offline!", WARN);
			return;
		}
		json response = json::parse(std::string(buffer));
		if(buffer[0]!='[')
		{
			_clientId = response["id"];
			//Log::verbose("ClientConnector", "ClientConnector id: "+std::to_string(_clientId));
		}
		else
		{
			std::cout << response << std::endl;
			while(survivors.size()<response.size())
				survivors.push_back(new Survivor(survivors[0]->getShader()));
			while(survivors.size()>response.size())
			{
				delete survivors.back();
				survivors.back() == nullptr;
				survivors.resize(survivors.size()-1);
			}

			for(int i=0; i<(int)response.size(); i++)
			{
				int survivorId = i;
				// Swap zero and clientId (I am 0)
				if(survivorId == _clientId) continue;
				if(survivorId == 0) survivorId = _clientId;

				json survivorState = response[i];
				std::cout << survivorState << std::endl;
				survivors[survivorId]->setJson(survivorState);
			}
		}
	}
}
