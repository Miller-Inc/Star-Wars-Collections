//
// Created by James Miller on 9/2/2025.
//

/// SW_Window.h
/// This file defines the SW_Window class, which is a simple window class for rendering images using Vulkan and SDL3.
///     This window is never actually used, it was just a test class for ImGui and SDL3 integration.

#pragma once
#include <map>
#include "EngineTypes/Core.h"
#include "Game/GameInstance.h"
#if PLATFORM_UNIX || PLATFORM_MACOS
#include <vulkan/vulkan.h>
#elif PLATFORM_WINDOWS
#include <Vulkan/vulkan.h>
#endif
#include "Vulkan/VulkanSetup.hpp"
#include "EngineTypes/String.h"

namespace MillerInc::GUI
{

    class SW_Window
    {
    public:
        SW_Window() = default;
        SW_Window(const SW_Window& other);
        void Init(); // Initialize ImGui context and SDL window
        void Init(const MString& WindowName, MillerInc::Game::GameInstance* GameInstance); // Initialize ImGui context and SDL window
        void Init(MillerInc::Game::GameInstance* GameInstance); // Initialize ImGui context and SDL window
        void TestWindow(); // Simple test window
        void AddImage(MImage* image); // Add an image to the window
        MImage* RemoveImage(const MString& ImageName);



    private:
        std::map<MString, MImage*> Textures{}; // Map to hold textures with string keys
        MString Name = "SW_Window";
        MillerInc::Game::GameInstance* mGameInstance = nullptr;

    };
}