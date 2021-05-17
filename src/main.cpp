#include <utility>
#include <Core/Client.h>

using namespace stocc;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    StonksClient client("ODQzODIyNzg2Njk4NTQzMTM1.YKJdNQ.KHhEdTHxvStb6TDGsiUw-Urnb0Y");
    client.run();

    system("pause");
}