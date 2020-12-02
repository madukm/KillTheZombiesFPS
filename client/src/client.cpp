#include <iostream>
#include <chrono>
#include <thread>

#include <mutex>

//#include "window.hpp"
#include "clientconnector.hpp"
#include "../../shared/gamestate.hpp" //TODO add in cmake to include <gamestate.hpp>
#include "../../shared/semaphore.hpp"

#define MOVE_RATE 1

class Client
{
    public:
    GameState state;
    GameObj player;
    std::mutex mutx;

    ClientConnector connector;
    std::thread update_thread;

    Client() 
    : connector (ClientConnector(state, 42069, "127.0.0.1", mutx))
    {
        update_thread = std::thread(&ClientConnector::update, &connector);
    }

    ~Client() //Join threads.
    {

    }

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
    }*/

    void onDraw()
    {
        while(true)
        {
            mutx.lock();
            state.a += 10;
            mutx.unlock();
            //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

//Client main. Should ask for server info and send info.
int main()
{
    //Semaphore sem(1);

    std::cout << "Client init!\n";
    Client game_client;

    std::thread client_thread(&Client::onDraw, &game_client);

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

