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

    GameObj(int objectId = -1, std::string name_ = "usuario x", glm::vec3 position = {0,1,0}, glm::vec3 rotation = {0,0,0}, glm::vec3 scale = {1,1,1}):
		_id(objectId), _name(name_), _health(1.0f), 
		_position(position), _rotation(rotation), _scale(scale), _power(1.0f),
		_fly(false), _front({1,0,0}), _moving_forward(0), _moving_left(0)
    {
	}

    GameObj(const GameObj &copied)
    {
        _name = copied._name;
        _health = copied._health;
        _power = copied._power;
        _id = copied._id;
        _position = copied._position;
        _rotation = copied._rotation;
        _scale = copied._scale;
        _fly = copied._fly;
        _front = copied._front;
        _moving_forward = copied._moving_forward;
        _moving_left = copied._moving_left;
    }

    static GameObj from_json(json parsed_obj)
    {
		int id = parsed_obj["id"];
		std::string name = parsed_obj["name"];
		glm::vec3 pos = glm::vec3(parsed_obj["position"][0], parsed_obj["position"][1], parsed_obj["position"][2]);
		glm::vec3 rot = glm::vec3(parsed_obj["rotation"][0], parsed_obj["rotation"][1], parsed_obj["rotation"][2]);
		glm::vec3 scale = glm::vec3(parsed_obj["scale"][0], parsed_obj["scale"][1], parsed_obj["scale"][2]);

        GameObj ret(id, name, pos, rot, scale);
		ret._health = parsed_obj["health"];
        ret._power = parsed_obj["power"];

		ret._fly = parsed_obj["fly"];
        ret._front = glm::vec3(parsed_obj["front"][0], parsed_obj["front"][1], parsed_obj["front"][2]);
		ret._moving_forward = parsed_obj["moving_forward"];
        ret._moving_left = parsed_obj["moving_left"];

        return ret;
    }

    json to_json()
    {
        json ret;
        ret["id"] = _id;	
        ret["name"] = _name;	
        ret["health"] = _health;
        ret["power"] = _power;	

		ret["position"] ={ _position.x, _position.y, _position.z };
        ret["rotation"] ={ _rotation.x, _rotation.y, _rotation.z };
        ret["scale"] ={ _scale.x, _scale.y, _scale.z };

		ret["fly"] = _fly;
        ret["front"] = { _front.x, _front.y, _front.z };
		ret["moving_forward"] = _moving_forward;
		ret["moving_left"] = _moving_left;
		
		return ret;
    }
	
	void decrease_health(const GameObj &hit_player)
	{
		_health -= hit_player._health;
	}
	
    std::string get_name() { return _name; }
    void set_name(std::string name) { _name = name; }

    int get_id() { return _id; }
    void set_id(int id){ _id = id; }

    glm::vec3 get_position() { return _position; }
    void set_position(glm::vec3 position) { _position = position; }

    glm::vec3 get_rotation() { return _rotation; }
    void set_rotation(glm::vec3 rotation) { _rotation = rotation; }

    glm::vec3 get_scale() { return _scale; }
    void set_scale(glm::vec3 scale) { _scale = scale; }

    float get_health() { return _health; }
    void set_health(float health) { _health = health; }

    float get_power() { return _power; }
    void set_power(float power) { _power = power; }

    bool get_fly() { return _fly; }
    void set_fly(bool fly) { _fly = fly; }

	glm::vec3 get_front() { return _front; }
    void set_front(glm::vec3 front) { _front = front; }

	int get_moving_forward() { return _moving_forward; }
    void set_moving_forward(int moving_forward) { _moving_forward = moving_forward; }

	int get_moving_left() { return _moving_left; }
    void set_moving_left(int moving_left) { _moving_left = moving_left; }

private:
    int _id;
	std::string _name;
    float _health;
    float _power;
    
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;

	bool _fly;
	glm::vec3 _front;
	int _moving_forward;
	int _moving_left;
	
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

		if(parsed_state["players"] != nullptr)
		{
            // Players is serialized as a vector,
            // but in fact it's an unordered map.
			for (auto body : parsed_state["players"])
			{
				GameObj _obj = GameObj::from_json(body);
				ret.players.insert(std::make_pair(_obj.get_id(), _obj));
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

        for (auto _player : players)
        {
		    ret["players"].push_back(_player.second.to_json());
        }

        return ret;
    }

    std::unordered_map<int, GameObj> players;
};

#endif
