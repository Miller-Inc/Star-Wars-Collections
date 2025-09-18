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

#define RESOURCE_RELATIVE_PATH "Resources/resources.json"

namespace MillerInc::Game
{
    class GameInstance;
    typedef std::function<void()> DrawWindowCallback;
    typedef std::function<void(GameInstance*)> InitWindowCallback;
    typedef std::function<void(float)> TickCallback;

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

        void Init(); // Initialize the game instance

        void Tick(float deltaTime);
        void Render(); // Render the game instance

        bool OpenImage(const MString& PathToImage, const MString& ImageName, const MVector2& Position, const MSize& Scale = {1.0f, 1.0f}); // Add an image to the texture map
        MImage* GetImage(const MString& ImageName); // Retrieve an image from the texture map
        MImage* OpenGetImage(const MString& PathToImage, const MString& ImageName, const MVector2& Position, const MSize& Scale = {1.0f, 1.0f});
        bool RemoveImage(const MString& ImageName);
        bool DeleteImage(const MString& ImageName);

        bool AddWindow(MWindow& newWindow); // Add a new window to the game instance

        bool LoadResources(const MString& WindowName);
        bool LoadResources();

    protected:
        void PreWindowInit();

    private:
        MVector2 WindowSize = {800, 600}; // Default window size
        std::map<MString, MImage> Textures{};
        std::map<MString, MWindow> Windows{};

        GPU::VulkanSetup *Setup = nullptr; // Pointer to Vulkan setup
        int Program(); // Main program loop
        bool IsRunning = false;

        ResourceLoader mResourceLoader{};

        void ParseResources();

    };
}