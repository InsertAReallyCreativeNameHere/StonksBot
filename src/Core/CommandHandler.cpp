#include "CommandHandler.h"

using namespace stocc;
using namespace SleepyDiscord;

std::string CommandHandler::prefix("!stocc ");
std::list<Command> CommandHandler::commandList;

CommandOutput CommandHandler::handleMessage(const Message& messageContent)
{
    sendMessage();
}