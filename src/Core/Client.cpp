#include "Client.h"

using namespace stocc;
using namespace SleepyDiscord;

void StonksClient::onMessage(Message message)
{
    sendMessage(message.channelID, "test" + message.author.username);
}