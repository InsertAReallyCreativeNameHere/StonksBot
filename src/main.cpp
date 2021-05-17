#include <utility>
#include <Core/Client.h>

using namespace stocc;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    StonksClient client(std::getenv("STONKS_TOKEN"));
    client.run();

    system("pause");
}