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

    GameObj(int objectId, std::string name_ = "usuario x", glm::vec3 position = {0,1,0}, glm::vec3 rotation = {0,0,0}):
		name(name_), health(1.0f), _position(position), _rotation(rotation)
    {
	}

    static GameObj from_json(json parsed_obj)
    {
		int id = parsed_obj["id"];
		std::string name = parsed_obj["name"];
		glm::vec3 pos = glm::vec3(parsed_obj["position"][0], parsed_obj["position"][1], parsed_obj["position"][2]);
		glm::vec3 rot = glm::vec3(parsed_obj["rotation"][0], parsed_obj["rotation"][1], parsed_obj["rotation"][2]);

        GameObj ret(id, name, pos, rot);
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
        ret["id"] = id;	
        ret["name"] = name;	
		
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
    float health;
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
        //ret.new_player_id = parsed_state["new_player_id"];

        //Parse objects.
		if(parsed_state["players"] != nullptr)
		{
			for (auto body : parsed_state["players"])
			{
				GameObj _obj = GameObj::from_json(body);
				ret.players.push_back(_obj);
			}
		}

        return ret;
    }

    json to_json()
    {
        json ret;
		ret["players"] = {};
        //ret["new_player_id"] = new_player_id;

        ret["zombies"] = {};

        for (GameObj &_player : players)
        {
		    ret["players"].push_back(_player.to_json());
        }

        return ret;
    }

    //int _player_id = -1; //Destination player ID
    std::vector<GameObj> players;
};

#endif
