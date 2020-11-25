#include <iostream>
#include <chrono>
#include <thread>

//#include "window.hpp"
#include "connector.hpp"
#include "gamestate.hpp"
#include "../../shared/semaphore.hpp"

#define MOVE_RATE 1

class Client
{
    public:
    GameState state;
    GameObj player;
    Semaphore &state_semaphore;

    Client(Semaphore &_state_sem)
    : state_semaphore(_state_sem)
    {}

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
        while(true)
        {
            printf("AA\n");
            state_semaphore.down();
            for (auto &body : state.bodies)
            {
                body.temp++;
                printf("body: %d\n", body.temp);
            }
            state_semaphore.up();
        }
    }
};

//Client main. Should ask for server info and send info.
int main()
{
    Semaphore sem(1);

    std::cout << "Client init!\n";
    Client game_client(sem);
    ClientConnector connector(game_client.state, 42069, "127.0.0.1", sem);

    std::thread update_thread(&ClientConnector::update, &connector);
    std::thread client_thread(&Client::onDraw, &game_client);

    while (1)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

