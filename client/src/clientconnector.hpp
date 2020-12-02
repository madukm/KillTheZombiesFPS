#include <vector>
#include <thread>
#include <chrono>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <mutex>

//Game
#include "../../shared/gamestate.hpp"
#include "../../shared/semaphore.hpp"
#include "../../shared/json.hpp"
#include "../../shared/logger.hpp"

using json = nlohmann::json; 
logger::logger client_connector_logger("clientconnector.hpp");

//Connects client to server.
class ClientConnector
{
    public:
	int sd;
	struct sockaddr_in serveraddr;
	GameState &update_state; //critical region
    Semaphore &state_semaphore; //Semaphore for critical region.


    char *update_buffer;

    ClientConnector(GameState &state, unsigned short port, std::string address, Semaphore &_state_semaphore) 
    : update_state(state), state_semaphore(_state_semaphore)
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
            client_connector_logger.log("Problem in tcp!", logger::log_type::WARN);
        }
        //should throw an exception...
        //
        update_buffer = new char[GameState::buf_size];
	}
	
	~ClientConnector()
	{
		close(sd);
	}

    void update()
    {
        //Get info and update state.
        json game_state_json;

        while (true)
        {
            memset(update_buffer, 0, GameState::buf_size);
<<<<<<< Updated upstream
            mutx.lock();
=======
            //TODO add logger messages here.

            state_semaphore.down();
>>>>>>> Stashed changes

            game_state_json = update_state.to_json(); //Must be in lock before reading and sending to server.
            strncpy(update_buffer, game_state_json.dump().c_str(), GameState::buf_size-1);

            //Send over tcp.
            //send(sd, update_buffer, GameState::buf_size, 0);
            write(sd, update_buffer, GameState::buf_size); 
            client_connector_logger.log("Sent data!", logger::log_type::DEBUG);

            //Receive updated values from server.
            //In this implementation, server is always right. (i suppose)
            //recv(sd, update_buffer, GameState::buf_size, 0);
            read(sd, update_buffer, GameState::buf_size);

            GameState received_state = GameState::from_json(json::parse(update_buffer));

            //printf("Client >> : %d\n", received_state.a);
            client_connector_logger.log("Received state", logger::log_type::DEBUG);
            update_state.a = received_state.a; //bem porquinho por enquanto.

<<<<<<< Updated upstream
            mutx.unlock();
=======

            state_semaphore.up();
>>>>>>> Stashed changes
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

