#include <functional>
#include <iostream>

#include "EngineTypes/Logger.h"
#include "Game/GameInstance.h"
#include "Windows/SW_Window.h"
#include "Network/NetworkManager.h"
#define NET_TEST false

// Main code
int main(int argc, char** argv)
{
#if NET_TEST
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD); // Initialize SDL
    MillerInc::Network::NetworkManager networkManager{};
    networkManager.Init();
    bool useServer = true; // Set to true to run as server, false for client

    std::cin >> useServer;
    if (!useServer)
    {
        MillerInc::MString ip;
        std::cin >> ip;
        if (SocketPtr socket = networkManager.CreateClient(ip.c_str(), 12345))
        {
            M_LOGGER(MillerInc::Logger::LogTemp, MillerInc::Logger::Debug, "Client connected to server at " + ip);
            char buffer[512];
            SDL_Delay(1500); // Wait for server to send data
            int bytesReceived = MillerInc::Network::NetworkManager::Receive(socket, buffer, sizeof(buffer));
            if (bytesReceived > 0)
            {
                M_LOGGER(MillerInc::Logger::LogTemp, MillerInc::Logger::Info, "Received from server: " + std::string(buffer));
            } else
            {
                M_LOGGER(MillerInc::Logger::LogTemp, MillerInc::Logger::Warning, "No data received from server.");
            }
        }
    } else
    {
        MillerInc::MArray<ServerPtr> servers;
        networkManager.CreateServer(12345, servers);

        for (auto& server : servers)
        {
            M_LOGGER(MillerInc::Logger::LogNetwork, MillerInc::Logger::Info, "Server listening on port 12345");
            networkManager.WaitForConnectionAsync(server, [](SocketPtr client)
            {
                NET_Address* Address = NET_GetStreamSocketAddress(client);
                NET_WaitUntilResolved(Address, 1500);

                M_LOGGER(MillerInc::Logger::LogNetwork, MillerInc::Logger::Info, "Client (" + std::string(Address->hostname ? Address->hostname : "Unknown") + ") connected to server asynchronously.");

                const char* welcomeMessage = "Welcome to the server!";
                MillerInc::Network::NetworkManager::Send(client, welcomeMessage, strlen(welcomeMessage) + 1);
            }, true);
        }
    }

    SDL_Delay(1000); // Keep the server running for 10 seconds to allow connections

    networkManager.WaitForThreads(); // Wait for all threads to finish}
    networkManager.Shutdown();
    SDL_Quit();


    return 0;
#endif

    MillerInc::Game::GameInstance gameInstance{};
    MillerInc::GUI::SW_Window window{};
    bool show_main_window = true;

    MillerInc::Game::MWindow mainWindow
    {
        "Main Window",
        {[&window]()
        {
            // M_LOGGER(MillerInc::Logger::LogTemp, MillerInc::Logger::Info, "Drawing Main Window");
            window.TestWindow();
        }},
        {[&window](MillerInc::Game::GameInstance* GameInstance)
        {
            window.Init(GameInstance);
        }},
        nullptr,
        &show_main_window
    };

    gameInstance.AddWindow(mainWindow);

    MillerInc::GUI::SW_Window window2{};
    bool show_main_window2 = true;

    MillerInc::Game::MWindow mainWindow2
    {
        "Main Window 2",
        {[&window2]()
        {
            // M_LOGGER(MillerInc::Logger::LogTemp, MillerInc::Logger::Info, "Drawing Main Window");
            window2.TestWindow();
        }},
        {[&window2](MillerInc::Game::GameInstance* GameInstance)
        {
            window2.Init("Main Window 2", GameInstance);
            window2.RemoveImage("Palm Tree");
            window2.AddImage(GameInstance->OpenGetImage("Resources/Textures/image.png", "Image", {30, 20}, {0.5f, 0.5f}));
        }},
        nullptr,
        &show_main_window
    };

    gameInstance.AddWindow(mainWindow2);

    gameInstance.Init();

    return 0;
}

