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
            /*
            for (auto client : active_clients)
            {
                //notify client
                //
                //get_message();
                // novo gamestate
            }*/
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

        active_clients.push_back(temp_new_client);
    }

    private:

    std::unordered_map<int, GameObj> _players;
    std::queue<GameMessage> message_queue; //kiwi
    std::vector<ServerConnector*> active_clients;
    Semaphore message_queue_semaphore;
    GameState _state;
};
