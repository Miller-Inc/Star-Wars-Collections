//
// Created by James Miller on 9/5/2025.
//

#pragma once
#include <functional>
#include "SDL3/SDL.h"
#include <SDL3_net/SDL_net.h>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "EngineTypes/Array.h"

struct NET_Address
{
    char *hostname;
    char *human_readable;
    char *errstr;
    SDL_AtomicInt refcount;
    SDL_AtomicInt status;  // This is actually a NET_Status.
    struct addrinfo *ainfo;
    NET_Address *resolver_next;  // a linked list for the resolution job queue.
};

typedef NET_StreamSocket* SocketPtr;
typedef NET_Server* ServerPtr;

namespace MillerInc::Network
{
    class NetworkManager
    {
    public:
        NetworkManager() = default;
        ~NetworkManager() = default;

        bool Init(); // Initialize the network manager
        void Shutdown(); // Shutdown the network manager

        bool CreateServer(int port, MArray<ServerPtr>& ServerOut); // Create a server
        SocketPtr CreateClient(const char* ip, int port); // Create a client
        SocketPtr Accept(const ServerPtr& server); // Accept a client connection
        bool WaitForConnectionAsync(const ServerPtr& socket, const std::function<void(SocketPtr)>& callback, bool continueListening = false); // Wait for a client connection asynchronously
        static bool Send(SocketPtr socket, const void* data, int len); // Send data to a socket
        static int Receive(SocketPtr socket, void* buffer, int len); // Receive data
        void WaitForThreads();

    private:
        bool isServer = false;
        bool isInit = false;

        MArray<ServerPtr> mListenServers; // List of socket sets
        MArray<SocketPtr> sockets; // List of sockets
        MArray<std::thread*> listenThreads; // List of listening threads

        std::mutex mutex;
        std::condition_variable condition;
        int running = 0;
    };
}
