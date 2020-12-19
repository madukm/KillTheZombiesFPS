#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netdb.h>
#include <atomic>

//game
#include "server_logic.hpp"

//TODO make this function as a thread.
int main()
{
	std::cout << "Server init\n";

    ServerLogic _logic;
    int incoming_descriptor, temp_descriptor; //Connection descriptor on newly created connection.
    struct sockaddr_in incoming_addr;
    socklen_t addr_size;

    //TODO configure incoming socket.
    incoming_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    incoming_addr.sin_family = AF_INET;
    incoming_addr.sin_port = htons(1337);
    incoming_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(incoming_descriptor, (struct sockaddr*) &incoming_addr, sizeof(incoming_addr));

    if (listen(incoming_descriptor, 4) != 0)
        printf("Error in listen."); //TODO add in logger.

    while(1) //Listen for connections.
    {
        addr_size = sizeof(incoming_addr);
        temp_descriptor = accept(incoming_descriptor, (struct sockaddr*) &incoming_addr, &addr_size);
        printf("Accepted connection!\n");

        _logic.add_client(temp_descriptor);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    //Wait for ctrl-C or ctrl-D

	return 0;
}
