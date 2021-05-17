#include "Client.h"

#include <Core/CommandHandler.h>

using namespace stocc;
using namespace SleepyDiscord;

void StonksClient::onMessage(Message message)
{
    if (!message.author.bot)
    {
        auto out = CommandHandler::handleMessage(message);
    }
}