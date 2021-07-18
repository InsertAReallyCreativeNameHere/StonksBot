#include <utility>
#include <Core/Client.h>
#include <Core/CommandHandler.h>

using namespace stocc;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    const char* secret = std::getenv("STONKS_TOKEN");
    if (secret != nullptr)
    {
        StonksClient client(secret, SleepyDiscord::USER_CONTROLED_THREADS);
        CommandHandler::client = &client;
        client.run();
    }
    else
    {
        std::cerr << "The environment variable STONKS_TOKEN is not set!\n";
        system("pause");
    }
}