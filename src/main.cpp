#include <utility>
#include <Core/Client.h>
#include <Core/CommandHandler.h>
#include <Core/Database.h>

using namespace stocc;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    const char* secret = std::getenv("STONKS_TOKEN");
    if (secret != nullptr)
    {
        Database::read();

        StonksClient client(secret, SleepyDiscord::USER_CONTROLED_THREADS);
        CommandHandler::client = &client;

        Restart:
        client.run();
        if (StonksClient::state.restartRequested)
        {
            // Do not show to any comittee members. Will be crucified otherwise.
            client.~StonksClient();
            new(&client) StonksClient(secret, SleepyDiscord::USER_CONTROLED_THREADS);
            StonksClient::state.restartRequested = false;
            client.sendMessage(StonksClient::state.channelID, "Server client restarted.");
            
            Database::write();
            goto Restart;
        }

        Database::write();
    }
    else
    {
        std::cerr << "The environment variable STONKS_TOKEN is not set!\n";
        system("pause");
    }
}