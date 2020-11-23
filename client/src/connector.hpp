#include <vector>
#include <thread>
#include <chrono>

#include "gamestate.hpp"
//#include "gamemessage.hpp"

//Connects client to server.

class ClientConnector
{
    public:

    ClientConnector(GameState &state) 
    : update_state(state)
    {}

    void update()
    {
        //Get info and update state.
        
        while (true)
        {
            update_state.bodies[0].temp += 1; 

            //Create udp connection.
            //Connect
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }
    }


    GameState &update_state; //critical region
};
