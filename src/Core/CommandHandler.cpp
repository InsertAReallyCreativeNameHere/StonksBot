#include "CommandHandler.h"

#include <cstring>
#include <map>
#include <Utility/Hash.h>

using namespace stocc;
using namespace Marble;
using namespace SleepyDiscord;

DiscordClient* CommandHandler::client = nullptr;

constexpr char prefix[] = "!stonks";
static std::map<uint64_t, CommandBehavior> commands
{
    {
        strhash("hello"),
        {
            [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
            {
                return { data.channelID, "Hello!", Embed::Flag::INVALID_EMBED };
            },
            { }
        }
    },
    {
        strhash("->"),
        {
            [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
            {
                auto& subcmds = commands[strhash("->")].subCommands;
                ++data.commandIndex;
                if (data.commandIndex < data.commandSplit.size())
                {
                    auto it = subcmds.find(strhash(data.commandSplit[data.commandIndex].c_str()));
                    if (it != subcmds.end())
                        return it->second.action(behavior, data);
                    else return { data.channelID, "Unknown advanced command!", Embed::Flag::INVALID_EMBED };
                }
                else return { data.channelID, "No advanced command supplied!", Embed::Flag::INVALID_EMBED };
            },
            {
                {
                    strhash("help"),
                    {
                        [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
                        {
                            return
                            {
                                data.channelID, "",
                                Embed
(R"(
{
    "title": "pain!",
    "description": "suffering"
}
)")
                            };
                        },
                        { }
                    }
                },
                {
                    strhash("quit"),
                    {
                        [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
                        {
                            return { data.channelID, "Goodbye!", Embed::Flag::INVALID_EMBED, [] { CommandHandler::client->quit(); } };
                        },
                        { }
                    }
                }
            }
        }
    }
};

std::vector<std::string> split(const std::string& str, const char* delimiter)
{
    std::vector<std::string> ret;
    size_t len = strlen(delimiter);

    if (len > 0)
    {
        size_t prevPos = 0;
        size_t pos = SIZE_MAX - len + 1;
        while ((pos = str.find(delimiter, pos + len)) != std::string::npos)
        {
            std::string sub = str.substr(prevPos + len, pos - prevPos - len);
            if (!sub.empty())
                ret.push_back(std::move(sub));
            prevPos = pos;
        }
        std::string sub = str.substr(prevPos + len, str.size() - prevPos - len);
        if (!sub.empty())
            ret.push_back(std::move(sub));
    }
    else
    {
        ret.reserve(len);
        for (auto it = delimiter; it != delimiter + len; ++it)
            ret.push_back(std::string(1, *it));
    }

    return ret;
}

void CommandHandler::handleMessage(const Message& messageContent)
{
    if (messageContent.content.starts_with(prefix))
    {
        std::vector<std::string> commandSplit(split(messageContent.content, " "));
        if (commandSplit.size() > 1) [[likely]]
        {
            for (auto it1 = commandSplit.begin(); it1 != commandSplit.end(); ++it1)
                for (auto it2 = it1->begin(); it2 != it1->end(); ++it2)
                    *it2 = std::tolower(*it2);

            CommandData data { messageContent.channelID, commandSplit, 1 };
            auto it = commands.find(strhash(commandSplit[1].c_str()));
            if (it != commands.end())
            {
                CommandOutput output = it->second.action(it->second, data);
                CommandHandler::client->sendMessage(output.channelID, output.message, output.embed);
                if (output.postOutputAction != nullptr)
                    output.postOutputAction();
            }
        }
    }
}