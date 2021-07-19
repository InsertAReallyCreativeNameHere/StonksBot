#pragma once

#include <json.hpp>

namespace stocc
{
    class Database final
    {
    public:
        Database() = delete;

        static void read();
        static void write();

        static nlohmann::json data;
    };
}