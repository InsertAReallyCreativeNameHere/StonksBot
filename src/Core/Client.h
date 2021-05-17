#pragma once

#include <sleepy_discord/sleepy_discord.h>

namespace stocc
{
    class StonksClient final : public SleepyDiscord::DiscordClient
    {
    public:
        using SleepyDiscord::DiscordClient::DiscordClient;
        void onMessage(SleepyDiscord::Message message) override
        {
            if (message.startsWith("shallot"))
                sendMessage(message.channelID, "test" + message.author.username);
        }
    };
}