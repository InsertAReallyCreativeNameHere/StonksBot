#pragma once

#include <initializer_list>
#include <list>
#include <string>
#include <Core/Client.h>
#include <sleepy_discord/sleepy_discord.h>

namespace stocc
{
    struct CommandOutput
    {
        SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID;
        std::string message;
        SleepyDiscord::Embed embed;
        void (*postOutputAction)() = nullptr;
    };

    struct CommandData
    {
        const SleepyDiscord::Message& message;
        std::vector<std::string>& commandSplit;
        size_t commandIndex;
    };

    struct CommandBehavior
    {
        std::string displayName;
        std::string description;
        CommandOutput (*action)(CommandBehavior& behavior, CommandData& data);
        std::unordered_map<std::string, CommandBehavior> subCommands;
    };

    class StonksClient;
    class CommandHandler final
    {
    public:
        CommandHandler() = delete;

        static StonksClient* client;

        static void handleMessage(const SleepyDiscord::Message& message);

        friend class StonksClient;
    };
}