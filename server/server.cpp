#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netdb.h>
#include <atomic>
#include "connector.hpp"
#include "../client/src/gamestate.hpp"

//SOCKET UDP:
//int sd;
//sd = socket(AF_INET, SOCK_DGRAM, 0);
//if(sd == -1){
//	perror("Error");
//	exit(1);
//}
//CONNECT lado cliente

class Server
{
	public:
		GameState state;
 
};
//We will need a thread.

//Server main application.
int main()
{
	std::cout << "Server init\n";
	Server game_server;
	ServerConnector connector(game_server.state, 42069);
   	 
    std::thread update_thread(&ServerConnector::update, &connector);

	return 0;
}
