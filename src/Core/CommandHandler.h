#pragma once

#include <initializer_list>
#include <list>
#include <string>
#include <sleepy_discord/sleepy_discord.h>

namespace stocc
{
    struct Command final
    {
        std::string name;
        std::string description;

        std::initializer_list<Command> subCommands;
    };

    struct CommandOutput
    {
        SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID;
        std::string message;
        SleepyDiscord::Embed embed;
    };

    class StonksClient;
    class CommandHandler final
    {
        static std::string prefix;
        static std::initializer_list<Command> commandList;
    public:
        CommandHandler() = delete;

        static CommandOutput handleMessage(const SleepyDiscord::Message& message);

        friend class StonksClient;
    };
}