#include "src/engine.hpp"

int main(int argc, char* argv[])
{
	Engine engine;
    if (engine.GetIsSetup())
    {
        // Begin Engine
        engine.Execute();
    }
	return 0;
}