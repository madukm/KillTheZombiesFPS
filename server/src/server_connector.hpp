#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>

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
        _update_state(update_state)
    {
        get_message_buf = new char[GameMessage::buf_size];
        send_state_buf = new char[GameState::buf_size];

        get_message_thread = std::thread(&ServerConnector::get_message, this);
        send_state_thread = std::thread(&ServerConnector::send_state, this);
    }
	
	~ServerConnector()
	{
		close(_socket);

        delete get_message_buf;
        delete send_state_buf;

        //TODO join threads.
	}

    // Get and enqueue message from client.
    void get_message()
    {
        while (true)
        {
            read(_socket, get_message_buf, GameMessage::buf_size);
            GameMessage aux = GameMessage::from_json(json::parse(get_message_buf));

            _m_queue_semaphore.down();
            _m_queue.push(aux);
            _m_queue_semaphore.up();
        }
    }

    // Sends GameState to client.
    void send_state()
    {
        while (true)
        {
            if (!_update_queue.empty())
            {
                strncpy(send_state_buf, _update_queue.front().to_json().dump().c_str(), GameState::buf_size-1);
                _update_queue.pop();
                printf("SEND JSON >>> %s\n", send_state_buf);
                write(_socket, send_state_buf, GameState::buf_size-1);
            }
        }
    }

    private:

	int _socket; //socket descriptor

    std::queue<GameMessage> &_m_queue; //Message queue to server.
    Semaphore &_m_queue_semaphore;
    
    std::queue<_update_state> _update_queue;
    
    char *get_message_buf;
    char *send_state_buf;

    std::thread get_message_thread;
    std::thread send_state_thread;
};
