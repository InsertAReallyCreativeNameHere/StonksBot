#include "CommandHandler.h"

#include <cstring>
#include <unordered_map>
#include <Utility/Hash.h>

using namespace stocc;
using namespace Marble;
using namespace SleepyDiscord;

StonksClient* CommandHandler::client = nullptr;

constexpr char prefix[] = "!stonks";
static std::unordered_map<std::string, CommandBehavior> commands
{
    {
        "hello",
        {
            "hello [Predecessor: !stonks]",
            "Test command that exists for legacy reasons.",
            [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
            {
                return { data.message.channelID, "Hello!", Embed::Flag::INVALID_EMBED };
            },
            { }
        }
    },
    {
        "->",
        {
            "-> [Predecessor: !stonks] (Advanced Command Prefix)",
            "Subcommands of this are reserved for developers.",
            [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
            {
                if (data.message.author.ID == 526324814017724446)
                {
                    auto& subcmds = commands["->"].subCommands;
                    ++data.commandIndex;
                    if (data.commandIndex < data.commandSplit.size())
                    {
                        auto it = subcmds.find(data.commandSplit[data.commandIndex]);
                        if (it != subcmds.end())
                            return it->second.action(behavior, data);
                        else return { data.message.channelID, "Unknown advanced command!", Embed::Flag::INVALID_EMBED };
                    }
                    else return { data.message.channelID, "No advanced command supplied!", Embed::Flag::INVALID_EMBED };
                }
                else return { data.message.channelID, "Volatile commands are only permitted for developers!", Embed::Flag::INVALID_EMBED };
            },
            {
                {
                    "help",
                    {
                        "help [Predecessor: !stonks ->] (Advanced)",
                        "Get a list of commands that can be used for this bot.",
                        [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
                        {
                            std::string embedJSON =
R"(
{
    "title": "Commands List",
    "description": "Here are a list of commmands:",
    "fields":
    [
)";

                            auto appendCommandToJSON = [](auto&& appendCommandToJSON, std::string& json, CommandBehavior& behavior) -> void
                            {
                                if (!behavior.displayName.empty())
                                {
                                    json
                                    .append("\n        {\n           \"name\": \"")
                                    .append(behavior.displayName)
                                    .append("\",\n            \"value\": \"")
                                    .append(behavior.description)
                                    .append("\"\n        },");
                                    for (auto it = behavior.subCommands.begin(); it != behavior.subCommands.end(); ++it)
                                        appendCommandToJSON(appendCommandToJSON, json, it->second);
                                }
                            };
                            for (auto it = commands.begin(); it != commands.end(); ++it)
                                appendCommandToJSON(appendCommandToJSON, embedJSON, it->second);
                            
                            embedJSON.pop_back();
                            embedJSON.append("\n    ]\n}\n");

                            std::cout << "The embed JSON is: \n" << embedJSON << '\n';

                            return
                            {
                                data.message.channelID, "",
                                Embed(embedJSON.c_str())
                            };
                        },
                        { }
                    }
                },
                {
                    "restart",
                    {
                        "restart [Predecessor: !stonks ->] (Advanced)",
                        "Requests the server client to restart.",
                        [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
                        {
                            StonksClient::state.restartRequested = true;
                            StonksClient::state.channelID = data.message.channelID;
                            return
                            {
                                data.message.channelID, "Restarting...", Embed::Flag::INVALID_EMBED,
                                []
                                {
                                    CommandHandler::client->quit();
                                }
                            };
                        },
                        { }
                    }
                },
                {
                    "quit",
                    {
                        "quit [Predecessor: !stonks ->] (Advanced)",
                        "Requests the server client to exit.",
                        [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
                        {
                            return
                            {
                                data.message.channelID,
                                (data.commandIndex += 2) < data.commandSplit.size() ?
                                "Warning: Ignoring unrecognised arguments past \"quit\" (-Wunknown-argument).\nGoodbye!" :
                                "Goodbye!", Embed::Flag::INVALID_EMBED,
                                [] { CommandHandler::client->quit(); }
                            };
                        },
                        { }
                    }
                },
                {
                    "pushStock",
                    {
                        "pushStock { Count : uint64_t } [Predecessor: !stonks ->] (Advanced)",
                        "Pushes a stock to the database.",
                        [](CommandBehavior& behavior, CommandData& data) -> CommandOutput
                        {
                            ++data.commandIndex;
                            if (data.commandIndex < data.commandSplit.size())
                            {
                                constexpr auto toInt = [](const std::string& base10Str, uint64_t& val) -> bool
                                {
                                    uint64_t prevVal = 0;
                                    val = 0;
                                    for (auto it = base10Str.cbegin(); it != base10Str.cend(); ++it)
                                    {
                                        prevVal = val;
                                        if (*it < '0' || *it > '9' || (val += *it - '0') <= prevVal) [[unlikely]]
                                            return false;
                                    }
                                    return true;
                                };

                                uint64_t count;
                                if (toInt(data.commandSplit[data.commandIndex], count))
                                {
                                    return
                                    {
                                        data.message.channelID,
                                        std::string("Pushed ")
                                        .append(std::to_string(69))
                                        .append(" amount of ")
                                        .append("stockname")
                                        .append(" to the database."),
                                        Embed::Flag::INVALID_EMBED
                                    };
                                }
                                else return { data.message.channelID, "Argument 1 \"Count\" is not a [uint64_t] (-Wconvertible-to).", Embed::Flag::INVALID_EMBED };
                            }
                            else return { data.message.channelID, "No \"Count\" argument given to \"pushStock\" (-Wmissing-argument).", Embed::Flag::INVALID_EMBED };
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

            CommandData data { messageContent, commandSplit, 1 };
            auto it = commands.find(commandSplit[1]);
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