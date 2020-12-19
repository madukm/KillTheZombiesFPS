#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <utility>
#include <tuple>
#include <glm/glm.hpp>

#include "json.hpp"

using json = nlohmann::json; 

enum obj_type
{
	PLAYER,
    ZOMBIE,
    EXTRA //Gift boxes.
};

class GameObj
{
    public:

    GameObj()
    {
		name = std::string("usuario x");
	}

	GameObj(unsigned int id)
	{
		name = std::string("usuario x");
		this->id = id;
	}

    static GameObj from_json(json parsed_obj)
    {
        GameObj ret;

        ret._position.x = parsed_obj["position"][0];
		ret._position.y = parsed_obj["position"][1];
		ret._position.z = parsed_obj["position"][2];
		
 		ret._rotation.x = parsed_obj["rotation"][0];
		ret._rotation.y = parsed_obj["rotation"][1];
		ret._rotation.z = parsed_obj["rotation"][2];
        
		ret.health = parsed_obj["health"];
        ret.power = parsed_obj["power"];

        return ret;
    }

    json to_json()
    {
        json ret;
		ret["position"] ={ _position.x, _position.y, _position.z };
        ret["rotation"] ={ _rotation.x, _rotation.y, _rotation.z };

        ret["health"] = health;
        ret["power"] = power;	
		
		return ret;
    }
	
	void decrease_health(const GameObj &hit_player)
	{
		health -= hit_player.health;
	}
	
    int get_id() { return id; }

    void set_id(int _id){ id = _id; }

    glm::vec3 get_position() { return _position; }
    void set_position(glm::vec3 position) { _position = position; }

    glm::vec3 get_rotation() { return _rotation; }
    void set_rotation(glm::vec3 rotation) { _rotation = rotation; }

private:
	std::string name;
    int health;
    int power;
    int id;
	
	glm::vec3 _position;
	glm::vec3 _rotation;
	
    //obj_type type;
};


class GameState
{
    public:

    static const int buf_size = 2000;

    GameState(){}
	GameState(const GameState &_state)
	{
		players = _state.players;
	}

    static GameState from_json(json parsed_state)
    {
        GameState ret;
        ret.new_player_id = parsed_state["new_player_id"];

        //Parse objects.
        for (auto body : parsed_state["players"])
        {
        	GameObj _obj = GameObj::from_json(body);
			ret.players.push_back(_obj);
		}

        return ret;
    }

    json to_json()
    {
        json ret;
		ret["players"] = {};
        ret["new_player_id"] = new_player_id;

        ret["zombies"] = {};

        for (GameObj &_player : players)
        {
		    ret["players"].push_back(_player.to_json());
        }

        return ret;
    }

    int _player_id = -1; //Destination player ID
    std::vector<GameObj> players;
};

#endif
