#pragma once

#include <initializer_list>
#include <list>
#include <string>
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
        SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID;
        std::vector<std::string>& commandSplit;
        size_t commandIndex;
    };

    struct CommandBehavior
    {
        CommandOutput (*action)(CommandBehavior& behavior, CommandData& data);
        std::map<uint64_t, CommandBehavior> subCommands;
    };

    class StonksClient;
    class CommandHandler final
    {
    public:
        CommandHandler() = delete;

        static SleepyDiscord::DiscordClient* client;

        static void handleMessage(const SleepyDiscord::Message& message);

        friend class StonksClient;
    };
}