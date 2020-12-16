#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>

#include "json.hpp"

using json = nlohmann::json; 

class GamePlayer
{
    public:

    GamePlayer()
    {
        name = std::string("OI\n");
    }

    static GamePlayer from_json(json parsed_obj)
    {
        GamePlayer ret;

        ret.pos_x = parsed_obj["pos_x"];
        ret.pos_y = parsed_obj["pos_y"];
        ret.pos_z = parsed_obj["pos_z"];

        ret.rot_x = parsed_obj["rot_x"];
        ret.rot_y = parsed_obj["rot_y"];
        ret.rot_z = parsed_obj["rot_z"];

        ret.health = parsed_obj["health"];
        ret.power = parsed_obj["power"];

        return ret;
    }

    json to_json()
    {
        json ret;

        ret["pos_x"] = pos_x;
        ret["pos_y"] = pos_y;
        ret["pos_z"] = pos_z;
                       
        ret["rot_x"] = rot_x;
        ret["rot_y"] = rot_y;
        ret["rot_z"] = rot_z;

        ret["health"] = health;
        ret["power"] = power;

        return ret;
    }

    int get_id()
    { return id; }

    void set_id(int _id)
    { id = _id; }

    void decrease_health(const GamePlayer &hit_player)
    {
        health -= hit_player.health;
    }

    private:

    std::string name;
    int health;
    int power;
    int id;

    float pos_x, pos_y, pos_z;
    float rot_x, rot_y, rot_z;
};

class GameState
{
    public:

    static const int buf_size = 2000;

    GameState()
    {}

    GameState(const GameState &_state)
    {
        this->players = _state.players;
    }

    static GameState from_json(json parsed_state)
    {
        GameState ret;

        //Parse objects.
        for (auto body : parsed_state["players"])
        {
            GamePlayer _player = GamePlayer::from_json(body);
            ret.players[_player.get_id()] = _player;
        }

        return ret;
    }

    json to_json()
    {
        json ret;

        ret["players"] = json::parse(players);

        return ret;
    }

    std::unordered_map<int, GamePlayer> players;
};

#endif
