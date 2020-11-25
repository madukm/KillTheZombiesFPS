//Runs server and client, and tests whether the received message was correct.

//Include game state utilities.

void test_server()
{
    
}

void test_client()
{
    
}

int main()
{
    std::thread s_thread(test_server);
    std::thread c_thread(test_client);
    
    s_thread.join();
    c_thread.join();
}