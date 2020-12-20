#define GLM_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include "../../shared/game_state.hpp"
#include "../../shared/game_message.hpp"
#include "server_connector.hpp"
#include "../../shared/semaphore.hpp"
#include "../../shared/logger.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <queue>


/* Server logic:
 *
 * */

class PotentialField{
	public:

	PotentialField(){}
	
	PotentialField(glm::vec2 initial, std::vector<glm::vec2> targets) :
		_initial(initial), _targets(targets)
	{
	}

	~PotentialField(){}

	glm::vec2 newDirection()

	{
		int repulsiveForce = 1;
		int attractiveForce = 2;
	
		//Repulsive (Obstacles)
		std::vector<glm::vec2> obstacleVectors;
		for(int i=0; i<int(_obstacles.size()); i++)
		{
			glm::vec2 vec;
			vec = _obstacles[i]-_initial;
			glm::normalize(vec);
			
			if(glm::length(vec)>0)
			{
				vec = vec * -float(repulsiveForce) / glm::length (_obstacles[i]-_initial);
				obstacleVectors.push_back(vec);
			}
		}
		
		glm::vec2 resultantRepulsive = {0,0};
		for(auto &vec : obstacleVectors){
			resultantRepulsive += vec;
		}
		
		//Attractive (Targets)
		glm::vec2 temp = (_targets[0]-_initial); 
		float minDist = glm::length(temp);
		int target = 0;
		for(int i=1; i<int(_targets.size()); i++)
		{
			float dist = glm::length(_targets[i]-_initial);
			if(dist < minDist){
			   	minDist = dist;
				target = i;
			}
		}
		glm::vec2 targetVector = _targets[target] - _initial;
		glm::vec2 resultantAttractive = targetVector * float(attractiveForce);

		glm::vec2 resultant2 = resultantAttractive;// + resultantRepulsive;
		glm::vec3 resultant = {resultant2.x, 0, resultant2.y};
		std::cout << "Result" << glm::to_string(resultant) << std::endl;
		return glm::normalize(resultant);
	}
	private:
		glm::vec2 _initial;
		std::vector<glm::vec2> _obstacles = {{0,50}, {0,-50}, {50,0}, {-50,0}, {-30,40}, {30,40}, {-30,-40}, {30,-40}, {0,0}, {-20,25}, {20,25}, {-20,25},{20,-25}};
		std::vector<glm::vec2> _targets;	
};

class ServerLogic
{
	public:
 
    ServerLogic(int zombie_n) :
        _max_zombie_n(zombie_n)
    {
		_log = new Logger("ServerLogic");
        curr_zombie_n = 0;
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
                    {
                        GameObj &dead_player =
                            _state.players[received_message._hitPlayer];
                        dead_player.decrease_health(_state.players[received_message._game_obj.get_id()]);
                        if(dead_player.get_health()<=0)
						{
							//TODO remove if dead...
						}
                    }

                    default:
                        break;
                }
            }

            // Spawn zombies.
            if (curr_zombie_n < _max_zombie_n)
            {
                int zomb_id;
                do
                {
                    zomb_id = rand() % 100;
                } while(_state.players.count(zomb_id) != 0);

                GameObj new_zombie(zomb_id);
                new_zombie.set_as_zombie();
				new_zombie.set_position({5,1,5});
				new_zombie.set_front({1,0,0});
				new_zombie.set_moving_forward(0);
				new_zombie.set_moving_left(0);

                _state.players.insert(std::make_pair(zomb_id, new_zombie));
				curr_zombie_n++;
            }

			std::vector<glm::vec2> playersPos;
			for (auto &[id, zombie] : _state.players)
            {
		   		if (!zombie.check_zombie()){
					glm::vec2 pos = {zombie.get_position().x, zombie.get_position().z};
					playersPos.push_back(pos);
				}
			}
            // Zombies wandering.
            for (auto &[id, zombie] : _state.players)
            {
		    
                if (zombie.check_zombie())
                {
                    //Wander
					if(playersPos.size() > 0){
						glm::vec2 zombiePos = {zombie.get_position().x, zombie.get_position().z};
						_potentialField = new PotentialField(zombiePos, playersPos);
						glm::vec3 vecDir = {_potentialField->newDirection().x, 0, _potentialField->newDirection().y};
						zombie.set_front(vecDir);
						zombie.set_moving_forward(1);
						zombie.set_moving_left(0);

						float speed = 5.0f;
						glm::vec3 position = zombie.get_position();
						position += vecDir * speed * 0.001f;// TODO calculate dt
						zombie.set_position(position);
                	}
					else{
						zombie.set_front({1,0,0});
						zombie.set_moving_forward(0);
						zombie.set_moving_left(0);
					}
				}
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
	
	PotentialField *_potentialField; 
    std::unordered_map<int, ServerConnector*> active_clients;
    std::queue<GameMessage> message_queue; //kiwi
    Semaphore message_queue_semaphore;
    const int _max_zombie_n;
    int curr_zombie_n;
    GameState _state;
	Logger* _log;
};

