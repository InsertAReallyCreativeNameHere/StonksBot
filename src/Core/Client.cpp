#include "Client.h"

#include <Core/CommandHandler.h>

using namespace stocc;
using namespace SleepyDiscord;

ClientState StonksClient::state;

void StonksClient::onMessage(Message message)
{
    if (!message.author.bot)
        CommandHandler::handleMessage(message);
}
