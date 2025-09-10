//
// Created by James Miller on 9/5/2025.
//

#include "Network/NetworkManager.h"
#include <vector>
#include <list>
#include <thread>
#include "EngineTypes/Logger.h"
#include "SDL3_net/SDL_net.h"


bool MillerInc::Network::NetworkManager::Init()
{
    if (isInit)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "NetworkManager is already initialized.");
        return true;
    }

    if (!SDL_Init(0))
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "SDL_Init: " ); //+ std::string(SDL_GetError()));
        return false;
    }

    if (!NET_Init())
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "SDLNet_Init: " ); // + std::string(SDL_GetError()));
        return false;
    }

    isInit = true;
    return true;
}

void MillerInc::Network::NetworkManager::Shutdown()
{
    if (!isInit)
    {
        return;
    }
    NET_Quit();
    isInit = false;
}

bool MillerInc::Network::NetworkManager::CreateServer(int port, MArray<ServerPtr>& ServerOut)
{
    ServerOut.Clear();

    if (!isInit)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "NetworkManager is not initialized. Call Init() first.");
        return false;
    }

    isServer = true;

    int numIps = 0;

    NET_Address** addresses = NET_GetLocalAddresses(&numIps);

    M_LOGGER(Logger::LogNetwork, Logger::Info, "Creating Server List of Size: " + std::to_string(numIps) + " on Port: " + std::to_string(port));

    for (int i = 0; i < numIps; i++)
    {
        NET_Address* localAddress = addresses[i];

        if (!localAddress)
        {
            M_LOGGER(Logger::LogNetwork, Logger::Warning, "Local address is null, skipping.");
            continue;
        }

        M_LOGGER(Logger::LogNetwork, Logger::Info, "IP address is: " + std::string(localAddress->hostname ? localAddress->hostname : "Unknown"));

        if (NET_Server* server = NET_CreateServer(localAddress, port))
        {
            mListenServers += server;
            M_LOGGER(Logger::LogNetwork, Logger::Info, "Server created on IP: " + std::string(localAddress->human_readable ? localAddress->human_readable : "Unknown") + " Port: " + std::to_string(port));

        }
        else
        {
            M_LOGGER(Logger::LogNetwork, Logger::Error, "Failed to create server on IP: " + std::string(localAddress->hostname ? localAddress->hostname : "Uknown") + " Port: " + std::to_string(port));
        }
    }

    if (numIps <= 0)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "No local IP addresses found, check network drivers/connections.");
        return false;
    }

    M_LOGGER(Logger::LogNetwork, Logger::Debug, "Total local IP addresses found: " + std::to_string(numIps));

    ServerOut += mListenServers;
    return true;
}

SocketPtr MillerInc::Network::NetworkManager::CreateClient(const char* ip, int port)
{
    NET_Address* address = NET_ResolveHostname(ip);
    NET_WaitUntilResolved(address, 1500);
    if (!address)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "SDLNet_ResolveHost failed: " + std::string(SDL_GetError()));
        return nullptr;
    }
    if (SocketPtr socket = NET_CreateClient(address, port))
    {
        sockets+= socket;
        M_LOGGER(Logger::LogNetwork, Logger::Info, "Client created and connected to IP: " + std::string(ip) + " Port: " + std::to_string(port));
        return socket;
    }
    else
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Failed to create client socket to IP: " + std::string(ip) + " Port: " + std::to_string(port) + " Error: " + std::string(SDL_GetError()));
        return nullptr;
    }
}

SocketPtr MillerInc::Network::NetworkManager::Accept(const ServerPtr& server)
{
    if (server != nullptr)
    {
        SocketPtr client;
        if (NET_AcceptClient(server, &client))
        {
            if (client != nullptr)
            {
                sockets += client;
                M_LOGGER(Logger::LogNetwork, Logger::Info, "Client connected to server. Total clients: " +
                    std::to_string(sockets.Len()));
                return client;
            }
        }
        // M_LOGGER(Logger::LogNetwork, Logger::Warning, "No incoming connections to accept at this time.");
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "Server pointer is null, cannot accept connections.");
    }
    return nullptr;
}

bool MillerInc::Network::NetworkManager::WaitForConnectionAsync(const ServerPtr& socket,
    const std::function<void(SocketPtr)>& callback, bool continueListening)
{
    if (socket == nullptr)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Server socket is null, cannot wait for connections.");
        return false;
    }

    auto* thread = new std::thread{([this, socket, callback, continueListening]()
        {
            // std::unique_lock<std::mutex> lock(mutex);
            ++running;
            try {
                M_LOGGER(Logger::LogNetwork, Logger::Info, "Waiting for connection...");
                while (true)
                {
                    SocketPtr client = Accept(socket);
                    if (client != nullptr)
                    {
                        callback(client);
                        if (!continueListening)
                            break;
                    }
                    SDL_Delay(250);
                }
            } catch (const std::exception& e)
            {
                --running;
                M_LOGGER(Logger::LogNetwork, Logger::Error, "Exception in connection thread: " + std::string(e.what()));
            }
            // std::unique_lock<std::mutex> unlock(mutex);
            --running;
        if (running == 0) condition.notify_one();

        }
    )};


    listenThreads.Add(thread);
    thread->detach(); // Detach the thread to run independently

    return true;
}

bool MillerInc::Network::NetworkManager::Send(const SocketPtr socket, const void* data, const int len)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot send data.");
        return false;
    }

    if (!data || len <= 0)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Failed to send data. Data not valid");
        return false;
    }

    M_LOGGER(Logger::LogNetwork, Logger::Debug, "Sending " + std::to_string(len) + " bytes of data.");
    NET_WriteToStreamSocket(socket, data, len);
    return true;
}

int MillerInc::Network::NetworkManager::Receive(const SocketPtr socket, void* buffer, const int len)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot receive data.");
        return -1;
    }
    if (!buffer || len <= 0)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Failed to receive data. Buffer not valid");
        return -1;
    }


    NET_WaitUntilInputAvailable((void**)&socket, 1, 5000); // Wait up to 5 seconds for data
    int bytesRead = NET_ReadFromStreamSocket(socket, buffer, len);

    if (bytesRead < 0)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Error receiving data: " + std::string(SDL_GetError()));
    } else if (bytesRead == 0)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No data received from server.");
    }

    return bytesRead;
}

void MillerInc::Network::NetworkManager::WaitForThreads()
{
    for (auto thread : listenThreads)
    {
        if (thread && thread->joinable())
        {
            thread->join();
            delete thread;
        }
    }
    listenThreads.Clear();
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock, [this]() { return running == 0; });
}
