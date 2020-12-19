#include <thread>
#include <chrono>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cerrno>

//Game
#include "../../shared/semaphore.hpp"
#include "../../shared/json.hpp"
#include "../../shared/logger.hpp"
#include "../../shared/game_state.hpp"
#include "../../shared/game_message.hpp"

using json = nlohmann::json; 

//Connects client to server.
class ClientConnector
{
    public:
	int sd;
	struct sockaddr_in serveraddr;

    Logger client_connector_logger;
    char *send_message_buffer;
    char *receive_state_buffer;

    /*this client ID*/
    ClientConnector(unsigned short port, std::string address) : 
        client_connector_logger(Logger("clientconnector.hpp"))
    {
		unsigned short _port = htons(port);
		if((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
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

        printf("File descriptor: %d\n", sd);
        //should throw an exception...
        receive_state_buffer = new char[GameState::buf_size];
        send_message_buffer = new char[GameMessage::buf_size];
	}

	~ClientConnector()
	{
		close(sd);
	}

    int get_socket() { return sd; }

    int send_init_sequence(std::string name) //Asks server for a slot.
    {
        //Get info and update state.
        json j_message;
        char buf[40] = {0};

        j_message["name"] = name; //Init
        strncpy(buf, j_message.dump().c_str(), 39);
        printf("Init sequence: %s\n", buf);
        write(sd, buf, 40); 
        printf("Rec init sequence: %s\n", buf);
        read(sd, buf,  40);
 
        return json::parse(buf)["your_id"];
    }

    void send_game_message(json j_message)
    {
        memset(send_message_buffer, 0, GameMessage::buf_size);
        strncpy(send_message_buffer, j_message.dump().c_str(), GameMessage::buf_size-1);

        client_connector_logger.log(std::string("Sending: ") + std::string(send_message_buffer), log_type::DEBUG);
        //Send over tcp.
        int bytes_written = write(sd, send_message_buffer, GameMessage::buf_size);
        if(bytes_written == -1) {
            printf("Oh dear, something went wrong with write()! %s\n", strerror(errno));
        }
        client_connector_logger.log(std::string("Bytes written: ") + std::to_string(bytes_written));
    }

    json receive_game_state() //ptr to game state maybe
    {
        //TODO add logger messages here.
        memset(receive_state_buffer, 0, GameState::buf_size);

        //Receive updated values from server.
        //In this implementation, server is always right. (i suppose)
        int bytes_read = read(sd, receive_state_buffer, GameState::buf_size);
        client_connector_logger.log(std::string("Bytes from server: ") + std::to_string(bytes_read));
        if(bytes_read == -1) {
                printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        }

        client_connector_logger.log(std::string("Received from server: ") + std::string(receive_state_buffer), log_type::INFO);

		json result;
		try
		{
			result = json::parse(receive_state_buffer);
        	//client_connector_logger.log(std::string("Received state: ") + std::string(receive_state_buffer), log_type::INFO);
		}
		catch(std::exception& e)
		{
			// Bad json format
			//std::cout << e. << std::endl;
			client_connector_logger.log(std::string("Bad JSON!"), WARN);
		}

        return result;
    }
};

