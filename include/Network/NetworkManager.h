//
// Created by James Miller on 9/5/2025.
//

/// NetworkManager.h
/// This file contains the declaration of the NetworkManager class, which manages network connections,
///     including creating servers and clients, sending and receiving data, and handling asynchronous connections.
///     It uses SDL_net for networking functionality and provides a higher-level interface for network operations.
///     The class supports both synchronous and asynchronous operations, making it suitable for various networked applications.
///     The NetworkManager class is designed to be thread-safe and can handle multiple connections simultaneously.
///     The main code has been adapted to fit into the engine's architecture, utilizing engine-specific types and logging mechanisms.


#pragma once
#include <functional> // For std::function
#include "SDL3/SDL.h" // For SDL types and functions
#include <SDL3_net/SDL_net.h> // For SDL_net types and functions
#include <condition_variable> // For std::condition_variable
#include <thread> // For std::thread
#include "EngineTypes/Core.h" // For engine core definitions

/// Taken from SDL_net documentation
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

/// Message struct for sending and receiving raw data
typedef struct message
{
    void *data;
    int len;
} MMessage;

/// Simple redefinitions for SDL_net types
typedef NET_StreamSocket* SocketPtr;

/// Simple redefinition for SDL_net server type
typedef NET_Server* ServerPtr;

namespace MillerInc::Network
{
    class NetworkManager
    {
    public:
        NetworkManager() = default;
        ~NetworkManager() = default;

        bool Init(bool* runningFlag = nullptr); // Initialize the network manager with an optional external running flag
        void Shutdown(); // Shutdown the network manager

        bool CreateServer(int port, MArray<ServerPtr>& ServerOut, MArray<NET_Address*>* AddressOut = nullptr); // Create a server
        SocketPtr CreateClient(const char* ip, int port); // Create a client
        SocketPtr Accept(const ServerPtr& server); // Accept a client connection
        bool WaitForConnectionAsync(const ServerPtr& socket, const std::function<void(SocketPtr)>& callback, bool continueListening = false); // Wait for a client connection asynchronously
        static bool Send(SocketPtr socket, const void* data, int len); // Send data to a socket
        static int Receive(SocketPtr socket, void* buffer, int len); // Receive data
        void WaitForThreads();

        SocketPtr CreateNamedSocket(const MString& name, const MString& ip, int port); // Create a named socket
        SocketPtr GetNamedSocket(const MString& name); // Get a named socket
        bool CloseNamedSocket(const MString& name); // Remove a named socket

        /// Send a string message over a socket by its pointer
        bool SendMessage(SocketPtr socket, const MString& message);

        /// Send a string message over a socket by its name
        bool SendMessage(MString& socket_name, const MString& message);

        /// Send raw data over a socket by its pointer
        bool Send(SocketPtr socket, MMessage& message);

        /// Send raw data over a socket by its name
        bool Send(const MString& socket_name, MMessage& message);

        /// Receive a string message from a socket by its pointer
        MString ReceiveMessage(SocketPtr socket);

        /// Receive a string message from a socket by its name
        MString ReceiveMessage(const MString& socket_name);

        /// Receive raw data from a socket by its pointer
        MMessage Receive(SocketPtr socket, size_t len); // Receive raw data

        /// Receive raw data from a socket by its name
        MMessage Receive(MString& socket_name, size_t len);

        /// Receive an integer from a socket
        int32_t ReceiveInt(SocketPtr socket);

        /// Receive an integer from a named socket
        int32_t ReceiveInt(const MString& socket_name);

        /// Receive a float from a socket
        float ReceiveFloat(SocketPtr socket);

        /// Receive a float from a named socket
        float ReceiveFloat(const MString& socket_name);

        /// Receive a double from a socket
        double ReceiveDouble(SocketPtr socket);

        /// Receive a double from a named socket
        double ReceiveDouble(const MString& socket_name);

        /// Receive a char from a socket
        char ReceiveChar(SocketPtr socket);

        /// Receive a char from a named socket
        char ReceiveChar(const MString& socket_name);

        /// Cleanup all sockets and servers
        void Cleanup();


    private:
        bool isServer = false;
        bool isInit = false;

        MArray<ServerPtr> mListenServers; // List of socket sets

        /// List of, as the client, a list of all the servers this device is connected to
        MArray<SocketPtr> mServerConnections;

        /// List of, as the server, a list of all the clients connected to this device
        MArray<SocketPtr> mClientSockets;
        MArray<std::thread*> listenThreads; // List of listening threads

        /// Map of the sockets that have been given a name for easy access
        MMap<MString, SocketPtr> mNamedSockets; // Map of named sockets

        // std::mutex mutex;
        // std::condition_variable condition;
        // int running = 0;
        bool* bIsRunning = nullptr; // Pointer to external running flag
    };
}
