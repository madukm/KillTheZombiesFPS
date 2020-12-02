#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netdb.h>
#include <atomic>

//game
#include "serverconnector.hpp"
#include "../../shared/gamestate.hpp"
/*
class Server
{
	public:
    Server()
    {}

    private:

    std::mutex game_state_mutex; // mutex on game state.
    std::vector<ServerConnector> client_connectors;
	GameState state; //General state
};
*/

int main()
{
	std::cout << "Server init\n";

    std::vector<ServerConnector*> active_clients;
    //Server game_server;
    GameState server_game_state;

    int incoming_descriptor, temp_descriptor; //Connection descriptor on newly created connection.
    struct sockaddr_in incoming_addr;
    socklen_t addr_size;

    //TODO configure incoming socket.
    incoming_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    incoming_addr.sin_family = AF_INET;
    incoming_addr.sin_port = htons(42069);
    incoming_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(incoming_descriptor, (struct sockaddr*) &incoming_addr, sizeof(incoming_addr));

    if (listen(incoming_descriptor, 4) != 0)
        printf("Error in listen."); //TODO add in logger.

    ServerConnector* temp_new_client;

    //TODO implement copy constructor.
    while(1) //Listen for connections.
    {
        //if () //if data available
        addr_size = sizeof(incoming_addr);
        temp_descriptor = accept(incoming_descriptor, (struct sockaddr*) &incoming_addr, &addr_size);
        
        temp_new_client = new ServerConnector(server_game_state, temp_descriptor);
        active_clients.push_back(temp_new_client);
            
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    //Freeing server.
    for (ServerConnector* client : active_clients)
    {
        //Terminate thread, run destructor...
        delete client;
    }

	return 0;
}
