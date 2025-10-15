//
// Created by James Miller on 9/4/2025.
//

/// GameInstance.h
/// This file contains the declaration of the GameInstance class, which manages the core game loop,
///     as well as window and texture management. This covers up most of the backend code, allowing
///     developers to focus on game logic and content. The main code has been taken and adapted from the
///     imgui Vulkan SDL3 example. Over time, the goal is to fully move this code to use the engine's
///     class structures and types, but for now, it is a hybrid of imgui example code and engine code.

#pragma once

#include <functional> // For std::function
#include "EngineTypes/Core.h" // Core engine types and utilities
#include "EngineTypes/ResourceLoader.h" // Resource loading utilities
#include "Network/NetworkManager.h" // Network management utilities

/// Define the path to the resources JSON file
#define RESOURCE_RELATIVE_PATH "Resources/resources.json"

namespace MillerInc::Game
{
    /// Forward declarations and type definitions

    /// Forward declaration of GameInstance since it is used in callback typedefs
    class GameInstance;

    /// Callback typedefs for window drawing, initialization, ticking, and client connections
    typedef std::function<void()> DrawWindowCallback;
    typedef std::function<void(GameInstance*)> InitWindowCallback;
    typedef std::function<void(float)> TickCallback;
    typedef std::function<void(int32_t, const MString&, SocketPtr)> ClientConnectedCallback;

    typedef struct m_window
    {
        MString Name;
        DrawWindowCallback DrawCallback;
        InitWindowCallback InitCallback;
        TickCallback Ticker;
        bool* OpenWindow = nullptr;

    } MWindow;

    class GameInstance
    {
    public:
        GameInstance() = default;
        ~GameInstance() = default;

        /// Prepare and run the game instance
        void Init(); // Initialize the game instance

        /// Update the game instance logic, called every frame
        void Tick(float deltaTime);

        /// Render the game instance, called every frame
        void Render(); // Render the game instance

        /// Texture Management

        /// Open an image and add it to the texture map
        bool OpenImage(const MString& PathToImage, const MString& ImageName, const MVector2& Position, const MSize& Scale = {1.0f, 1.0f}); // Add an image to the texture map

        /// Get an image from the texture map
        MImage* GetImage(const MString& ImageName); // Retrieve an image from the texture map

        /// Open an image if it doesn't exist, otherwise return the existing image
        MImage* OpenGetImage(const MString& PathToImage, const MString& ImageName, const MVector2& Position, const MSize& Scale = {1.0f, 1.0f});

        /// Remove an image from the texture map
        bool RemoveImage(const MString& ImageName);

        /// Delete an image from memory and the texture map
        bool DeleteImage(const MString& ImageName);

        /// Window Management

        /// Add a new window to the game instance
        bool AddWindow(MWindow& newWindow); // Add a new window to the game instance

        /// Load resources for a specific window
        std::map<MString, MImage*> LoadResources(const MString& WindowName);

        /// Load all resources
        bool LoadResources();

        /// Network Management

        /// Connect to a server
        bool ConnectToServer(const MString& name, const MString& ip, int32_t port);

        /// Return a reference to the network manager
        Network::NetworkManager *GetNetworkManager() { return &mNetworkManager; }

        /// Start a server on a specific port asynchronously and return a list of the addresses in string form that the server is listening on
        MArray<MString> StartServerAsync(int32_t port, const ClientConnectedCallback& callback = nullptr);

        /// Exits the main loop and shuts down the game instance
        void StopMainLoop() { RunLoop = false; }

    protected:
        /// Pre-window initialization (called before any windows are created) (Called once)
        void PreWindowInit();

        /// Cleanup resources (called on shutdown) (Called once)
        void Cleanup();

    private:
        MVector2 mWindowSize = {800, 600}; // Default window size
        std::map<MString, MImage> mTextures{};
        std::map<MString, MWindow> mWindows{};

        GPU::VulkanSetup *mSetup = nullptr; // Pointer to Vulkan setup
        int Program(); // Main program loop
        bool IsRunning = false;
        bool RunLoop = true;

        /// Resource loader instance, handles loading resources from disk and labeling them for use
        ResourceLoader mResourceLoader{};

        void ParseResources();

        /// Manage network connections
        Network::NetworkManager mNetworkManager{};

        /// Client names by their port
        std::map<int32_t, std::vector<MString>> mClients{};

        /// All Connected Clients by name
        MMap<MString, SocketPtr> mNamedClients;


    };
}