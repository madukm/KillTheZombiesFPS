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
        //GameMessage received_message;
        //bool new_message;

        //while(1)
        //{
        //    message_queue_semaphore.down();
        //    if (!message_queue.empty())
        //    {
        //        received_message = message_queue.front();
        //        message_queue.pop();
        //        new_message = true;
        //    } else new_message = false;
        //    message_queue_semaphore.up();

        //    if (new_message)
        //    {
        //        //tratar msg
        //        switch (received_message._type)
        //        {
        //            case MOVE:
        //            {
        //                GameObj &temp_obj = 
        //                    _players[received_message._game_obj.get_id()];
        //                temp_obj.set_position(received_message._game_obj.get_position());
        //                temp_obj.set_rotation(received_message._game_obj.get_rotation());
        //                break;
        //            }

        //            case HIT:
        //                //Encontrou no state
        //                //MAtou alguem.
        //                //killed_ids.push_back(iddequemmooreu);

        //            default:
        //                break;
        //        }
        //    }

        //    //spawn zombies.
        //    //altera gamestate
        //    
        //    //notifica os clientes.
        //    for (auto client : active_clients)
        //    {
        //        //notify client
        //        GameState temp_state;
        //        //temp_state._player_id = client.first;
        //        //client.second->_update_queue.push();
        //    }
        //}
    }

    void add_client(int new_client_descriptor)
    {
        ServerConnector *temp_new_client;

        //Reference to gamestate, 
        //message_queue reference, semaphore
        temp_new_client = new ServerConnector(new_client_descriptor,
                                              message_queue,
                                              message_queue_semaphore,
                                              _state);
        
        active_clients[new_client_descriptor] = temp_new_client;
		_state.players.push_back(GameObj(new_client_descriptor));
    }

	void delete_disconnected()
	{
		for(auto client : active_clients)
		{
			if(client.second->get_connected() == false)
			{
				delete client.second;
				active_clients.erase(client.first);

				for(int i=0; i<(int)_state.players.size(); i++)
				{
					if(_state.players[i].get_id() == client.first)
					{
						_state.players.erase(_state.players.begin()+i);
					}
				}
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
