#include "register.h"

int main(int argc, char const *argv[])
{
    RegisterBot("toto", [](int input) { return input * 2; });
    RegisterBot("tutu", [](int input) { return input + 2; });

    Engine engine;
    engine.run();
    return 0;
}
