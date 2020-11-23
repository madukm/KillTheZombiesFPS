#include <iostream>
#include <chrono>
#include <thread>

//#include "window.hpp"
#include "connector.hpp"
#include "gamestate.hpp"

#define MOVE_RATE 1

class Client
{
    public:
    GameState state;
    GameObj player;
 
    /*
    onKeyPressed() //Qualquer coisa 
    {
        switch (key)
        {
            case "W": player.y += MOVE_RATE;
            case "S": player.y -= MOVE_RATE;

            case "A": player.x -= MOVE_RATE;
            case "D": player.x += MOVE_RATE;
        }
    }
    */

    void onDraw()
    {
        for (auto body : state.bodies)
        {
            printf("body: %d\n", body.temp);
        }
    }
};

//Client main. Should ask for server info and send info.
int main()
{
    std::cout << "Client init!\n";
    Client game_client;
    ClientConnector connector(game_client.state);

    std::thread update_thread(&ClientConnector::update, &connector);

    while (1)
    {
        game_client.onDraw();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

