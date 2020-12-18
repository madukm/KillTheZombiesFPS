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

private:
	std::string name;
    int health;
    int power;
    int id;
	
	glm::vec3 _position;
	glm::vec3 _rotation;
	
    obj_type type;

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

        //Parse objects.
        for (auto body : parsed_state["players"])
        {
        	GameObj _obj = GameObj::from_json(body);
			ret.players[_obj.get_id()] = _obj;
		}

        return ret;
    }

    json to_json()
    {
        json ret;
		ret["players"] = json::parse(players);
        return ret;
    }

	std::unordered_map<int, GameObj> players;

};

#endif
