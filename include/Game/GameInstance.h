//
// Created by James Miller on 9/4/2025.
//

#pragma once

#include <functional>
#include <map>
#include "EngineTypes/Image.h"
#include "EngineTypes/String.h"
#include "EngineTypes/Vector.h"
#include "EngineTypes/ResourceLoader.h"
#include "Network/NetworkManager.h"

#define RESOURCE_RELATIVE_PATH "Resources/resources.json"

namespace MillerInc::Game
{
    class GameInstance;
    typedef std::function<void()> DrawWindowCallback;
    typedef std::function<void(GameInstance*)> InitWindowCallback;
    typedef std::function<void(float)> TickCallback;
    typedef std::function<void(int32_t, const MString&, SocketPtr)> ClientConnectedCallback;

    typedef struct mWindow
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
        MVector2 WindowSize = {800, 600}; // Default window size
        std::map<MString, MImage> Textures{};
        std::map<MString, MWindow> Windows{};

        GPU::VulkanSetup *Setup = nullptr; // Pointer to Vulkan setup
        int Program(); // Main program loop
        bool IsRunning = false;
        bool RunLoop = true;

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