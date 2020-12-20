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
#include <chrono>

/* Server logic:
 *
 * */
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;

class PotentialField
{
	public:

	PotentialField(){}
	
	PotentialField(glm::vec2 initial, std::vector<glm::vec2> targets, std::vector<glm::vec2> zombies) :
		_initial(initial), _targets(targets), _zombies(zombies)
	{
	}

	~PotentialField(){}

	glm::vec2 newDirection()

	{
		int repulsiveForce = 20;
		int repulsiveForceZombie = 5;
		int attractiveForce = 10;
	
		//Repulsive (Obstacles)
		glm::vec2 resultantRepulsive = {0,0};
		std::vector<glm::vec2> obstacleVectors;
		for(int i=0; i<int(_obstacles.size()); i++)
		{
			glm::vec2 vec;
			vec = glm::normalize(_obstacles[i]-_initial);
			
			if(glm::length(vec)>0)
			{
				vec = vec * -float(repulsiveForce) / glm::length (_obstacles[i]-_initial);
				resultantRepulsive += vec;
			}
		}

		//Repulsive (Zombie)
		for(auto zombiePos : _zombies)
		{
			glm::vec2 dirVector = glm::normalize(zombiePos - _initial);
			if(glm::length(dirVector)>0)
			{
				glm::vec2 vec = -dirVector * float(repulsiveForceZombie)/ glm::length (zombiePos-_initial);
				resultantRepulsive += vec;
			}
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
		glm::vec2 targetVector = glm::normalize(_targets[target] - _initial);
		glm::vec2 resultantAttractive = targetVector * float(attractiveForce);

		glm::vec2 resultant2 = resultantAttractive + resultantRepulsive;
		glm::vec3 resultant = {resultant2.x, resultant2.y, 0};

        //_log.log(std::string("Result") + glm::to_string(resultant), DEBUG);
		
        return glm::normalize(resultant);
	}
        static Logger _log;

	private:
		glm::vec2 _initial;
		std::vector<glm::vec2> _obstacles = {{0,50}, {0,-50}, {50,0}, {-50,0}, {-30,40}, {30,40}, {-30,-40}, {30,-40}, {0,0}, {-20,25}, {20,25}, {-20,25},{20,-25}};
		std::vector<glm::vec2> _targets;
		std::vector<glm::vec2> _zombies;
};

Logger PotentialField::_log = Logger("PotentialField");

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
        Clock::time_point prev_time = Clock::now(), curr_time;

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

            state_semaphore.down();
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
							_state.players.erase(dead_player.get_id());
                    }

                    default:
                        break;
                }
            }

            //If server should zombies position (difficulty).
            curr_time = Clock::now();
            if (milliseconds(5) <
                std::chrono::duration_cast<milliseconds>(curr_time - prev_time))
            {
                prev_time = curr_time;
                // Spawn zombies.
                if (curr_zombie_n < _max_zombie_n)
                {
                    int zomb_id;
                    do
                    {
                        zomb_id = rand()%100+100;
                    } while(_state.players.count(zomb_id) != 0);

                    GameObj new_zombie(zomb_id);
                    new_zombie.set_as_zombie();
    				new_zombie.set_position({float(rand()%100-50),.5,float(rand()%100-50)});
    				new_zombie.set_front({1,0,0});
    				new_zombie.set_scale({.5,.5,.5});
    				new_zombie.set_moving_forward(0);
    				new_zombie.set_moving_left(0);

                    _state.players.insert(std::make_pair(zomb_id, new_zombie));
    				curr_zombie_n++;
                }


                /*  The following logic is a simple IA so the 
                    game is not boring.
                */

    			std::vector<glm::vec2> playersPos;
    			std::vector<glm::vec2> zombiesPos;
    			for (auto &[id, zombie] : _state.players)
                {
    					glm::vec2 pos = {zombie.get_position().x, zombie.get_position().z};
    		   		if (!zombie.check_zombie())
    					playersPos.push_back(pos);
					else
    					zombiesPos.push_back(pos);
    			}

                // Zombies wandering.
                for (auto &[id, zombie] : _state.players)
                {
    		    
                    if (zombie.check_zombie())
                    {
                        //Wander
    					if(playersPos.size() > 0){
    						glm::vec2 zombiePos = {zombie.get_position().x, zombie.get_position().z};
    						_potentialField = PotentialField(zombiePos, playersPos, zombiesPos);
    						glm::vec3 vecDir = {_potentialField.newDirection().x, 0, _potentialField.newDirection().y};
    						zombie.set_front(vecDir);
    						zombie.set_moving_forward(1);
    						zombie.set_moving_left(0);

							// Update rotation
							glm::vec3 basex = glm::vec3(1,0,0);
							glm::vec3 basey = glm::vec3(0,0,1);
							float rotAngle = glm::dot(glm::vec3(vecDir), basex)/M_PI*180.0f;
							if(glm::dot(glm::vec3(vecDir), basey)>0)
    							zombie.set_rotation(glm::vec3(0.0f,rotAngle,0.0f));
							else
    							zombie.set_rotation(glm::vec3(0.0f,-rotAngle-180.0f,0.0f));

							// Update position
    						float speed = 10.0f;
    						glm::vec3 position = zombie.get_position();
    						position += vecDir * speed * 0.01f;// TODO calculate dt
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
            state_semaphore.up();
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
                                              state_semaphore,
                                              _state);
        
        active_clients[new_client_descriptor] = temp_new_client;
    }

	void delete_disconnected()
	{
		for(auto& [id, client] : active_clients)
		{
			if(client->get_connected() == false)
			{
				delete client;

                state_semaphore.down();
                if(_state.players.count(id) != 0)
                {
                    _state.players.erase(id);
                }
                state_semaphore.up();

				active_clients.erase(id);
				return;
			}
		}
	}

    private:
	
	PotentialField _potentialField;
    std::unordered_map<int, ServerConnector*> active_clients;
    std::queue<GameMessage> message_queue; //kiwi
    Semaphore message_queue_semaphore;
    Semaphore state_semaphore;
    const int _max_zombie_n;
    int curr_zombie_n;
    GameState _state;
	Logger* _log;
};

