#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <chrono>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include "../client/src/gamestate.hpp"

class ServerConnector
{
	public:
	int sd;
	char buffer[30];
	struct sockaddr_in clientaddr, serveraddr;
    GameState &update_state;

	ServerConnector(GameState &state, unsigned short port) 
    : update_state(state)
    {
		if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		{
			printf("socket()");
			exit(1);
		}	
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = port;
    	serveraddr.sin_addr.s_addr = INADDR_ANY;
	}
	
	~ServerConnector()
	{
		close(sd);
	}

    void update()
    {
        //Get info and update state.
        
        while (true)
        {
            update_state.bodies[0].temp += 1; 

            //Create udp connection.
			
			if(bind(sd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
			{
				printf("bind()");
				exit(2);
			}

			unsigned int namelen;
			namelen = sizeof(serveraddr);
			if(getsockname(sd, (struct sockaddr *) &serveraddr, &namelen) < 0){
				printf("getsockname()");
				exit(3);
			}
			
			printf("Port assigned is %d\n", ntohs(serveraddr.sin_port));
			unsigned int client_addr_size = sizeof(clientaddr);
			//Connect
			if(recvfrom(sd, buffer, 30, 0, (struct sockaddr *) &clientaddr, &client_addr_size) < 0)
			{
				printf("recvfrom()");
				exit(4);
			}
			printf("Received message %s from domain %s port %d internet address %s\n", buffer,
				   	(clientaddr.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
       				ntohs(clientaddr.sin_port),
       				inet_ntoa(clientaddr.sin_addr));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
    }

};
