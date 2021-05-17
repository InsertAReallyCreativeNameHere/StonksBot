#pragma once

#include <list>
#include <string>
#include <sleepy_discord/sleepy_discord.h>

namespace stocc
{
    struct Command final
    {
        std::string name;
        std::string description;

        std::list<Command> subCommands;
    };

    class StonksClient;
    class CommandHandler final
    {
        static std::string prefix;
        static std::list<Command> commandList;
    public:
        CommandHandler() = delete;

        struct CommandOutput
        {
            SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID;
            std::string message;
            SleepyDiscord::Embed embed;
        };
        static CommandOutput handleMessage(const SleepyDiscord::Message& message);

        friend class StonksClient;
    };
}