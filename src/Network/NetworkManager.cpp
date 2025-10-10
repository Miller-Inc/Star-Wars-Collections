//
// Created by James Miller on 9/5/2025.
//

#include "Network/NetworkManager.h"
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

    // Set the flag false to signal threads to stop
    isInit = false;

    WaitForThreads();

    NET_Quit();
}

bool MillerInc::Network::NetworkManager::CreateServer(const int port, MArray<ServerPtr>& ServerOut)
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
        mServerConnections+= socket;
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
                mClientSockets += client;
                M_LOGGER(Logger::LogNetwork, Logger::Info, "Client connected to server. Total clients: " +
                    std::to_string(mClientSockets.Len()));
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
                while (isInit)
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


    NET_WaitUntilInputAvailable((void**)&socket, 1, -1);
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

SocketPtr MillerInc::Network::NetworkManager::CreateNamedSocket(const MString& name, const MString& ip, int port)
{
    if (mNamedSockets.contains(name))
    {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "Socket with name " + name + " already exists. Use a different name.");
        return mNamedSockets[name];
    }

    if (auto socket = CreateClient(ip.c_str(), port))
    {
        mNamedSockets.emplace(name, socket);
        return socket;
    }

    return nullptr;
}

SocketPtr MillerInc::Network::NetworkManager::GetNamedSocket(const MString& name)
{
    if (mNamedSockets.contains(name))
    {
        return mNamedSockets[name];
    }

    return nullptr;
}

bool MillerInc::Network::NetworkManager::CloseNamedSocket(const MString& name)
{
    if (mNamedSockets.contains(name))
    {
        auto socket = mNamedSockets[name];
        if (socket)
        {
            NET_DestroyStreamSocket(socket);
        } else {
            M_LOGGER(Logger::LogNetwork, Logger::Warning, "Socket pointer is null when closing named socket: " + name);
            return false; // Socket pointer is null
        }

        mNamedSockets.erase(name);

        if (auto idx = mClientSockets.Find(socket); idx != -1)
        {
            mClientSockets.RemoveAt(idx);
        } else {
            M_LOGGER(Logger::LogNetwork, Logger::Warning, "Socket not found in client list when closing named socket: " + name);
            return false; // Socket was not found in client list
        }
        return true;
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + name);
    }
    return false;
}

bool MillerInc::Network::NetworkManager::SendMessage(SocketPtr socket, const MString& message)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot send message.");
        return false;
    }
    return Send(socket, message.c_str(), message.size() + 1); // +1 for null terminator
}

bool MillerInc::Network::NetworkManager::SendMessage(MString& socket_name, const MString& message)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return SendMessage(socket, message);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return false;
}

bool MillerInc::Network::NetworkManager::Send(SocketPtr socket, MMessage& data)
{
    return Send(socket, data.data, data.len);
}

bool MillerInc::Network::NetworkManager::Send(const MString& socket_name, MMessage& data)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return Send(socket, data);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return false;
}

MillerInc::MString MillerInc::Network::NetworkManager::ReceiveMessage(SocketPtr socket)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot receive message.");
        return "";
    }

    auto [data, len] = Receive(socket, MAX_STRING_MESSAGE_LEN);
    if (len > 0 && data != nullptr)
    {
        MString receivedMessage(static_cast<char*>(data));
        free(data); // Free the allocated memory
        return receivedMessage;
    }
    free(data);
    return "";
}

MillerInc::MString MillerInc::Network::NetworkManager::ReceiveMessage(const MString& socket_name)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return ReceiveMessage(socket);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return "";
}

MMessage MillerInc::Network::NetworkManager::Receive(SocketPtr socket, size_t len)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot receive data.");
        return {nullptr, 0};
    }

    char buffer[len]; // Buffer to hold received data
    int bytesReceived = Receive(socket, buffer, len);
    if (bytesReceived > 0)
    {
        MMessage message;
        message.data = malloc(bytesReceived);
        memcpy(message.data, buffer, bytesReceived);
        message.len = bytesReceived;
        return message;
    }

    return {nullptr, 0};
}

MMessage MillerInc::Network::NetworkManager::Receive(MString& socket_name, size_t len)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return Receive(socket, len);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return {nullptr, 0};
}

int32_t MillerInc::Network::NetworkManager::ReceiveInt(SocketPtr socket)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot receive integer.");
        return 0;
    }

    int32_t value = 0;
    int bytesRead = Receive(socket, &value, sizeof(int32_t));
    if (bytesRead == sizeof(int32_t))
    {
        return value;
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "Failed to receive full integer. Bytes read: " + std::to_string(bytesRead));
    }
    return 0;
}

int32_t MillerInc::Network::NetworkManager::ReceiveInt(const MString& socket_name)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return ReceiveInt(socket);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return 0;
}

float MillerInc::Network::NetworkManager::ReceiveFloat(SocketPtr socket)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot receive float.");
        return 0.0f;
    }

    float value = 0.0f;
    int bytesRead = Receive(socket, &value, sizeof(float));
    if (bytesRead == sizeof(float))
    {
        return value;
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "Failed to receive full float. Bytes read: " + std::to_string(bytesRead));
    }
    return 0.0f;
}

float MillerInc::Network::NetworkManager::ReceiveFloat(const MString& socket_name)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return ReceiveFloat(socket);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return 0.0f;
}

double MillerInc::Network::NetworkManager::ReceiveDouble(SocketPtr socket)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot receive double.");
        return 0.0;
    }

    double value = 0.0;
    int bytesRead = Receive(socket, &value, sizeof(double));
    if (bytesRead == sizeof(double))
    {
        return value;
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "Failed to receive full double. Bytes read: " + std::to_string(bytesRead));
    }
    return 0.0;
}

double MillerInc::Network::NetworkManager::ReceiveDouble(const MString& socket_name)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return ReceiveDouble(socket);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return 0.0;
}

char MillerInc::Network::NetworkManager::ReceiveChar(SocketPtr socket)
{
    if (!socket)
    {
        M_LOGGER(Logger::LogNetwork, Logger::Error, "Socket is null, cannot receive char.");
        return '\0';
    }
    char value = '\0';
    int bytesRead = Receive(socket, &value, sizeof(char));
    if (bytesRead == sizeof(char))
    {
        return value;
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "Failed to receive full char. Bytes read: " + std::to_string(bytesRead));
    }
    return '\0';
}

char MillerInc::Network::NetworkManager::ReceiveChar(const MString& socket_name)
{
    if (auto socket = GetNamedSocket(socket_name); socket != nullptr)
    {
        return ReceiveChar(socket);
    } else {
        M_LOGGER(Logger::LogNetwork, Logger::Warning, "No named socket found with name: " + socket_name);
    }
    return '\0';
}

void MillerInc::Network::NetworkManager::Cleanup()
{
    static bool closed = false; // Prevent multiple cleanup calls
    if (closed)
        return;

    closed = true;

    WaitForThreads();

    for (auto& [name, socket] : mNamedSockets)
    {
        CloseNamedSocket(name); // Close each named socket
    }

    for (auto& socket : mClientSockets)
    {
        if (socket)
        {
            NET_DestroyStreamSocket(socket);
        }
    }
    mClientSockets.Clear();

    for (auto& server : mListenServers)
    {
        if (server)
        {
            NET_DestroyServer(server);
        }
    }
    mListenServers.Clear();
    Shutdown();
}
