#include "../../shared/logger.hpp"
#include "client.hpp"

int main()
{
    //client_log.log("Started!", logger::log_type::INFO);
    Client client;
    
    std::thread sender_thread = std::thread(&Client::messageSender, &client);
    std::thread receiver_thread = std::thread(&Client::stateReceiver, &client);
    
	client.run();

    //std::thread client_thread(&Client::onDraw, &game_client);
    return 0;
}
