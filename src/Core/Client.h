#pragma once

#include <sleepy_discord/sleepy_discord.h>

namespace stocc
{
    struct ClientState
    {
        bool restartRequested = false;
        SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID = "0";
    };

    class StonksClient final : public SleepyDiscord::DiscordClient
    {
    public:
        using SleepyDiscord::DiscordClient::DiscordClient;

        static ClientState state;

        void onMessage(SleepyDiscord::Message message) override;
    };
}