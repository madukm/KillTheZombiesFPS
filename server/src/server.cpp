#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netdb.h>
#include <atomic>

//game
#include "server_logic.hpp"

void listen_connections();
void remove_disconnected_clients();

ServerLogic _logic;
int incoming_descriptor, temp_descriptor; //Connection descriptor on newly created connection.
struct sockaddr_in incoming_addr;
socklen_t addr_size;

//TODO make this function as a thread.
int main()
{
	std::cout << "Server init\n";

    //TODO configure incoming socket.
    incoming_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    incoming_addr.sin_family = AF_INET;
    incoming_addr.sin_port = htons(1338);
    incoming_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(incoming_descriptor, (struct sockaddr*) &incoming_addr, sizeof(incoming_addr));

    if (listen(incoming_descriptor, 4) != 0)
    {
        printf("Error in listen."); //TODO add in logger.
    }

    std::thread listen_connections_t(listen_connections);
    std::thread remove_disconnected_clients_t(remove_disconnected_clients);

    while(1) //Listen for connections.
    {

    }

	listen_connections_t.join();
	remove_disconnected_clients_t.join();


	// TODO ServerLogic run

    //Wait for ctrl-C or ctrl-D

	return 0;
}

void listen_connections()
{
	while(1)
	{
        addr_size = sizeof(incoming_addr);
        temp_descriptor = accept(incoming_descriptor, (struct sockaddr*) &incoming_addr, &addr_size);
        printf("Accepted connection at descriptor %d\n", temp_descriptor);

        _logic.add_client(temp_descriptor);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void remove_disconnected_clients()
{
	while(1)
	{
		_logic.delete_disconnected();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

