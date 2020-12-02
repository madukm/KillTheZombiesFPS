#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>

#include "json.hpp"

using json = nlohmann::json; 

class GameObj
{
    public:

    std::string name;
    int temp;

    GameObj()
    {
        name = std::string("OI\n");
        temp = 0;
    }

    static GameObj from_json(json parsed_obj)
    {
        GameObj ret;

        ret.temp = parsed_obj["temp"];

        return ret;
    }

    json to_json()
    {
        json ret;

        ret["temp"] = this->temp;

        return ret;
    }

    //float x, y, z;

    /*enum 
    {
        PLAYER,
        ZOMBIE,
        EXTRA //Gift boxes.
    } type;*/
};

class GameState
{
    public:

    static const int buf_size = 2000;

    GameState()
    {
        //GameObj temp;
        //bodies.push_back(temp);
    }

    static GameState from_json(json parsed_state)
    {
        GameState ret;

        ret.a = parsed_state["a"];
        
        //Parse objects.
        for (auto body : parsed_state["bodies"])
        {
            ret.bodies.push_back(GameObj::from_json(body));
        }

        return ret;
    }

    json to_json()
    {
        json ret;

        ret["a"] = a;

        json bodies = json::array();

        for (auto body : this->bodies)
        {
            bodies.push_back(body.to_json());
        }

        ret["bodies"] = bodies;

        return ret;
    }

    int a;

    private:

    std::vector<GameObj> bodies; //To be rendered.
};

#endif
