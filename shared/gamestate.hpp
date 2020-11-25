#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>

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

    GameState()
    {
        GameObj temp;
        bodies.push_back(temp);
    }

    /* something like that?
    static GameState fromJson(char* buffer)
    {

    }
    */

    std::vector<GameObj> bodies; //To be rendered.
};

#endif
