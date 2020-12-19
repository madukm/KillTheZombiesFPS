#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <cerrno>

// Networking stuff.
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>

#include "../../shared/game_state.hpp"
#include "../../shared/game_message.hpp"
#include "../../shared/semaphore.hpp"
#include "../../shared/logger.hpp"

/* This class represents an active client
 * to the server perspective.
 * Receives a file descriptor, accepted by the server
 * and initializes a thread. 
 */

class ServerConnector
{
	public:

    ServerConnector(int socket,
                    std::queue<GameMessage> &m_queue,
                    Semaphore &m_queue_semaphore,
                    GameState &update_state) :
        _socket(socket),
        _m_queue(m_queue),
        _m_queue_semaphore(m_queue_semaphore),
        _update_state(update_state),
		_connected(true)
    {
		log = new Logger("ServerConnector");
		log->log(std::string("Client connected: ") + std::to_string(socket), INFO);

        get_message_buf = new char[GameMessage::buf_size];
        send_state_buf = new char[GameState::buf_size];

        get_message_thread = std::thread(&ServerConnector::get_message, this);
        send_state_thread = std::thread(&ServerConnector::send_state, this);

        printf("Cliend FD: %d\n", socket);
    }
	
	~ServerConnector()
	{
        //TODO stop threads.
		get_message_thread.join();
		send_state_thread.join();

        delete get_message_buf;
        delete send_state_buf;
		delete log;

		close(_socket);
	}

    // Get and enqueue message from client.
    void get_message()
    {
        while (_connected)
        {
            int read_size = read(_socket, get_message_buf, GameMessage::buf_size);

			if(read_size>0)
			{
				GameMessage aux = GameMessage::from_json(json::parse(get_message_buf));

				_m_queue_semaphore.down();
				_m_queue.push(aux);
				_m_queue_semaphore.up();
			}
			else
			{
				// Client disconnected
				log->log(std::string("Client disconnected: ") + std::to_string(_socket), INFO);
				_connected = false;
			}
        }
    }

    // Sends GameState to client.
    void send_state()
    {
		// TODO stop this thread when deleting object
        while (_connected)
        {
            strncpy(send_state_buf, _update_state.to_json().dump().c_str(), GameState::buf_size-1);
            log->log(std::string("Sending ") + send_state_buf, DEBUG);
			int write_size = write(_socket, send_state_buf, GameState::buf_size);
			if(write_size<=0)
			{
				// Client disconnected
				log->log(std::string("Client disconnected: ") + std::to_string(_socket), INFO);
				_connected = false;
			}
        	std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

	bool get_connected() const { return _connected; }

    private:

	int _socket; //socket descriptor
    std::queue<GameMessage> &_m_queue;
    Semaphore &_m_queue_semaphore;
    GameState &_update_state; //Game reference
    
    char *get_message_buf;
    char *send_state_buf;

    std::thread get_message_thread;
    std::thread send_state_thread;

	Logger* log;

	bool _connected;
};
