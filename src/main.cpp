#include <utility>
#include <Core/Client.h>

using namespace stocc;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    StonksClient client("ODQzODIyNzg2Njk4NTQzMTM1.YKJdNQ.0KH0xEBYBL6vLE7Q1iYVCuQlfpk", std::thread::hardware_concurrency());
    client.run();

    system("pause");
}