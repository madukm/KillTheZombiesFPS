#include "../../shared/game_state.hpp"
#include "../../shared/game_message.hpp"
#include "server_connector.hpp"
#include "../../shared/semaphore.hpp"

#include <queue>

/* Server logic:
 *
 * */

class ServerLogic
{
	public:
 
    ServerLogic()
    {}

    ~ServerLogic()
    {
        //Freeing server.
        for (ServerConnector* client : active_clients)
        {
            //Terminate thread, run destructor...
            delete client;
        }
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
                            _players[received_message.game_obj.get_id()];
                        temp_obj.set_position(received_message.game_obj.get_position());
                        temp_obj.set_rotation(received_message.game_obj.get_rotation());
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
                temp_state._player_id = client.first;
                client.second->_update_queue.push();
            }
        }
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

        int new_id;
        do
         {
             /* code */
         } while (active_clients.count(new_id) != 0); 
        
        active_clients[new_id] = temp_new_client;

        //simply send the value to the client.
    }

    private:

    std::unordered_map<int, GameObj> _players;
    std::unordered_map<int, ServerConnector*> active_clients;
    std::queue<GameMessage> message_queue; //kiwi
    Semaphore message_queue_semaphore;
    GameState _state;
};
