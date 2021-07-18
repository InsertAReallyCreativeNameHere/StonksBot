#include <utility>
#include <Core/Client.h>

using namespace stocc;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    StonksClient client(argc > 0 ? argv[0] : "argc <= 0", std::thread::hardware_concurrency());
    client.run();

    system("pause");
}