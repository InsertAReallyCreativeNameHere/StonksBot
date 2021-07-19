#include "Database.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;
using namespace stocc;

nlohmann::json Database::data;

void Database::read()
{
    if (fs::exists(fs::current_path().append("database.json")))
    {
        std::ifstream in(fs::current_path().append("database.json"), std::ios::binary);
        std::streambuf* rawBuffer = in.rdbuf();

        in.seekg(0,std::ios::end);
        std::streampos length = in.tellg();
        in.seekg(0,std::ios::beg);

        char* block = new char[sizeof(char) * length + 1];
        block[sizeof(char) * length] = 0;
        rawBuffer->sgetn(block, sizeof(char) * length);

        try
        { Database::data = std::move(nlohmann::json::parse(block)); }
        catch(nlohmann::detail::exception& ex)
        { std::cerr << ex.what() << '\n'; }

        delete[] block;
    }
}
void Database::write()
{
    std::ofstream out(fs::current_path().append("database.json"));
    std::string outStr = Database::data.dump();
    out.write(outStr.c_str(), sizeof(char) * outStr.size());
}