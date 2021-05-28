#include "CommandHandler.h"

using namespace stocc;
using namespace SleepyDiscord;

std::string CommandHandler::prefix("!stocc ");
std::initializer_list<Command> CommandHandler::commandList;

CommandOutput CommandHandler::handleMessage(const Message& messageContent)
{
    return { messageContent.channelID, "wow", Embed::Flag::INVALID_EMBED };
}