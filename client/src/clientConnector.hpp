#include <vector>
#include <thread>
#include <chrono>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

//Game
#include "../../shared/semaphore.hpp"
#include "../../shared/json.hpp"
#include "../../shared/logger.hpp"
#include "../../shared/gamestate.hpp"

using json = nlohmann::json; 
Logger client_connector_logger("clientconnector.hpp");

//Connects client to server.
class ClientConnector
{
    public:
	int sd;
	struct sockaddr_in serveraddr;

    char *update_buffer;

    ClientConnector(unsigned short port, std::string address) 
    {
		unsigned short _port = htons(port);
		if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("socket()");
			exit(1);
		}

		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = _port;
    	serveraddr.sin_addr.s_addr = inet_addr(address.c_str());

        if (connect(sd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0)
        {
            client_connector_logger.log("Problem in tcp!", log_type::WARN);
        }
        //should throw an exception...
        update_buffer = new char[GameState::buf_size];
	}
	
	~ClientConnector()
	{
		close(sd);
	}

    void send_game_message(json &j_message)
    {
        //Get info and update state.
        json game_state_json;

        memset(update_buffer, 0, GameState::buf_size);
        strncpy(update_buffer, j_message.dump().c_str(), GameState::buf_size-1);

        //Send over tcp.
        write(sd, update_buffer, GameState::buf_size); 
        client_connector_logger.log("Sent data!", log_type::DEBUG);
    }

    json receive_game_state() //ptr to game state maybe
    {
        //TODO add logger messages here.
        memset(update_buffer, 0, GameState::buf_size);

        //Receive updated values from server.
        //In this implementation, server is always right. (i suppose)
        read(sd, update_buffer, GameState::buf_size);

        client_connector_logger.log("Received state", log_type::DEBUG);

        return json::parse(update_buffer);
    }
};

