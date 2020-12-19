#include "../../shared/game_state.hpp"
#include "../../shared/game_message.hpp"
#include "server_connector.hpp"
#include "../../shared/semaphore.hpp"
#include "../../shared/logger.hpp"

#include <queue>

/* Server logic:
 *
 * */

class ServerLogic
{
	public:
 
    ServerLogic()
    {
		_log = new Logger("ServerLogic");
	}

    ~ServerLogic()
    {
        //Freeing server.
        for (auto client : active_clients)
        {
            //Terminate thread, run destructor...
			if(client.second != nullptr)
            	delete client.second;
        }

		delete _log;
    }

    //different thread
    void run()
    {
        GameMessage received_message;
        bool new_message;

        while(1)
        {
            message_queue_semaphore.down();
            if (!message_queue.empty())
            {
                received_message = message_queue.front();
                message_queue.pop();
                new_message = true;
            } else new_message = false;
            message_queue_semaphore.up();

            if (new_message)
            {
                //tratar msg
                switch (received_message._type)
                {
                    case MOVE:
                    {
                        GameObj &temp_obj = 
                            _state.players[received_message._game_obj.get_id()];
                        temp_obj.set_position(received_message._game_obj.get_position());
                        temp_obj.set_rotation(received_message._game_obj.get_rotation());
                        temp_obj.set_scale(received_message._game_obj.get_scale());
                        temp_obj.set_health(received_message._game_obj.get_health());
                        temp_obj.set_power(received_message._game_obj.get_power());
                        temp_obj.set_name(received_message._game_obj.get_name());

                        temp_obj.set_fly(received_message._game_obj.get_fly());
                        temp_obj.set_front(received_message._game_obj.get_front());
                        temp_obj.set_moving_forward(received_message._game_obj.get_moving_forward());
                        temp_obj.set_moving_left(received_message._game_obj.get_moving_left());
                        break;
                    }

                    case HIT:
                        //Encontrou no state
                        //MAtou alguem.
                        //killed_ids.push_back(iddequemmooreu);

                    default:
                        break;
                }
            }

            //spawn zombies.
            //altera gamestate
            
            //notifica os clientes.
            for (auto client : active_clients)
            {
                //notify client
                GameState temp_state;
                //temp_state._player_id = client.first;
                //client.second->_update_queue.push();
            }
        }
    }

    void add_client(int new_client_descriptor)
    {
        ServerConnector *temp_new_client;
        char buf[40];
        json incoming_j_obj, outgoing_j_obj;

        // Wait for client name, and send client ID.
        read(new_client_descriptor, buf, 40); //read(_socket, get_message_buf, GameMessage::buf_size);
        incoming_j_obj = json::parse(buf);
        _log->log(std::string("New player enters the ring: ")
            + std::string(incoming_j_obj["name"]));

        outgoing_j_obj["your_id"] = new_client_descriptor;
        strncpy(buf, outgoing_j_obj.dump().c_str(), 40);
        write(new_client_descriptor, buf, 40);

        GameObj added_player = GameObj (new_client_descriptor, 
                                        incoming_j_obj["name"]);
        _state.players.insert(std::make_pair(new_client_descriptor, added_player));

        temp_new_client = new ServerConnector(new_client_descriptor,
                                              message_queue,
                                              message_queue_semaphore,
                                              _state);
        
        active_clients[new_client_descriptor] = temp_new_client;
    }

	void delete_disconnected()
	{
		for(auto& client : active_clients)
		{
			if(client.second->get_connected() == false)
			{
				delete client.second;
				client.second = nullptr;

                if(_state.players.count(client.first) != 0)
                {
                    _state.players.erase(client.first);
                }

				active_clients.erase(client.first);
				return;
			}
		}
	}

    private:

    std::unordered_map<int, ServerConnector*> active_clients;
    std::queue<GameMessage> message_queue; //kiwi
    Semaphore message_queue_semaphore;
    GameState _state;
	Logger* _log;
};
