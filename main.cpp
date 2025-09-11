#include <functional>
#include <iostream>

#include "EngineTypes/Logger.h"
#include "EngineTypes/ResourceLoader.h"
#include "Game/GameInstance.h"
#include "Network/NetworkManager.h"
#include "Windows/MainWindow.h"
#include "Windows/NetworkPlayWindow.h"
#define NET_TEST false
#define DEMO false

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

    MillerInc::ResourceLoader resourceLoader{};
    if (!resourceLoader.LoadResources("Resources/resources.json"))
    {
        M_LOGGER(MillerInc::Logger::LogCore, MillerInc::Logger::Error, "Failed to load resources.");
        return -1;
    }

    MillerInc::Game::GameInstance gameInstance{};

    MillerInc::GUI::MainWindow window{};
    bool show_main_window = true;

    MillerInc::Game::MWindow mainWindow
    {
        "Main Window",
        {[&window]()
        {
            window.Draw();
        }},
        {[&window, &resourceLoader](MillerInc::Game::GameInstance* GameInstance)
        {
            resourceLoader.OpenResources(GameInstance, std::vector<std::string>{"MainWindow"});
            window.Init(GameInstance);
        }},
        nullptr,
        &window.isOpen
    };

    gameInstance.AddWindow(mainWindow);

    gameInstance.Init();

    return 0;
}

