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

using json = nlohmann::json; 

//Connects client to server.
class ClientConnector
{
    public:
	int sd;
	struct sockaddr_in serveraddr;
	GameState &update_state; //critical region
    //Semaphore &state_semaphore; //Semaphore for critical region.

    std::mutex &mutx;

    char *update_buffer;

    ClientConnector(GameState &state, unsigned short port, std::string address, std::mutex &_mutx) 
    : update_state(state), mutx(_mutx)
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

        if (connect(sd, (struct sockaddr*)&serveraddr, sizeof(serveraddr) != 0))
        {
            printf("Problem in tcp!\n");
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
            //TODO add logger messages here.

            mutx.lock();
            game_state_json = update_state.to_json(); //Must be in lock before reading and sending to server.
            //strcpy(update_buffer, game_state_json.dump(), );

            mutx.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

