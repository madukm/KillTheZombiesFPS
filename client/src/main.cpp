#include "../../shared/logger.hpp"
#include "client.hpp"

int main()
{
    //client_log.log("Started!", logger::log_type::INFO);
    Client client;
	client.run();

    //std::thread client_thread(&Client::onDraw, &game_client);

    //while (1)
    //{
    //    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //}

    return 0;
}
