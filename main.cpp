#include "include/core/Application.h"

#include <iostream>

int main()
{
    Application app;

    if (!app.Initialize())
    {
        std::cerr
            << "Failed to initialize Minecraft Alpha."
            << std::endl;

        return EXIT_FAILURE;
    }

    app.Run();

    return EXIT_SUCCESS;
}