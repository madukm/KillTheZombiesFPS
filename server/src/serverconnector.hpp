#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <chrono>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>

#include "../../shared/gamestate.hpp"

/* This class represents an active client
 * to the server perspective.
 * Receives a file descriptor, accepted by the server
 * and initializes a thread. 
 */

class ServerConnector
{
	public:

    ServerConnector(GameState &_update_state, int _socket) 
    : socket(_socket), update_state(_update_state)
    {
        update_buf = new char[GameState::buf_size];
        update_thread = std::thread(&ServerConnector::update, this);
    }
	
	~ServerConnector()
	{
		close(socket);
        delete update_buf;
        //TODO join thread.
	}

    void update()
    {
        GameState received_state;

        printf("Server update started\n");

        while (true)
        {
            //Lock state mutex.

            //Receive state.
            //recv(socket, update_buf, GameState::buf_size, 0);
            read(socket, update_buf, GameState::buf_size);
            printf("%s\n", update_buf);

            received_state = GameState::from_json(json::parse(update_buf));

            //Do some processing.
            received_state.a += 1000;
            strncpy(update_buf, received_state.to_json().dump().c_str(), GameState::buf_size-1);

            //Unlock state mutex.
            
            //Send state.
            //send(socket, update_buf, GameState::buf_size, 0);
            write(socket, update_buf, GameState::buf_size);
        }
    }

    public:
	int socket; //socket descriptor
    GameState &update_state; //Game reference
    std::thread update_thread; //TODO initialize

    private:

    char *update_buf;
};
