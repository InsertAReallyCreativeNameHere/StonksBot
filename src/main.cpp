#include <utility>
#include <Core/Client.h>

using namespace stocc;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    StonksClient client("ODQzODIyNzg2Njk4NTQzMTM1.YKJdNQ.lCXCUN0TSJ2TJ8n5TWZ72y77VvM", SleepyDiscord::USER_CONTROLED_THREADS);
    client.run();

    system("pause");
}