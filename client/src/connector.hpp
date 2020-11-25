#include <vector>
#include <thread>
#include <chrono>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <netdb.h>
#include "gamestate.hpp"
#include <unistd.h>
//#include "gamemessage.hpp"

//Connects client to server.

class ClientConnector
{
    public:
	int sd;
	char buffer[30];
	struct sockaddr_in serveraddr;
	GameState &update_state; //critical region

    ClientConnector(GameState &state, unsigned short port, std::string address) 
    : update_state(state)
    {
		unsigned short _port = htons(port);
		if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		{
			printf("socket()");
			exit(1);
		}	
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = _port;
    	serveraddr.sin_addr.s_addr = inet_addr(address.c_str());
	}
	
	~ClientConnector()
	{
		close(sd);
	}

    void update()
    {
        //Get info and update state.
        
        while (true)
        {
    		//update_state.bodies[0].temp += 1; 

			int temp; 

			snprintf(buffer, 30, "%d", update_state.bodies[0].temp);

			if(sendto(sd, buffer, 30, 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
			{
				printf("sendto()");
				exit(2);
			}
			
			unsigned int server_addr_size = sizeof(serveraddr);
			if(recvfrom(sd, buffer, 30, 0, (struct sockaddr *)&serveraddr, &server_addr_size) < 0)
			{
				printf("recvfrom()");
				exit(4);
			}

			printf("Received message %s from domain %s port %d internet address %s\n", buffer,
				  (serveraddr.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
				  ntohs(serveraddr.sin_port),
				  inet_ntoa(serveraddr.sin_addr));
			sscanf(buffer, "%d", &update_state.bodies[0].temp);
            //Create udp connection.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
    }


};
