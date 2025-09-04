//
// Created by James Miller on 9/2/2025.
//

#pragma once
#include <map>
#include "EngineTypes/Image.h"
#if defined(__linux__) || defined(__APPLE__)
#include <vulkan/vulkan.h>
#elif defined(_WIN32)
#include <Vulkan/vulkan.h>
#endif
#include "Vulkan/VulkanSetup.hpp"
#include "EngineTypes/String.h"

namespace MillerInc::GUI
{

    class SW_Window
    {
    public:
        SW_Window(const SW_Window& other);
        explicit SW_Window(MillerInc::GPU::VulkanSetup* setup);
        void Init(); // Initialize ImGui context and SDL window
        void Init(const MString& WindowName); // Initialize ImGui context and SDL window
        void TestWindow() const; // Simple test window
        MillerInc::GPU::VulkanSetup* Setup = nullptr;

        bool AddImage(const MString& PathToImage, const MString& ImageName, const MVector2& Position); // Add an image to the texture map
        MImage* GetImage(const MString& ImageName); // Retrieve an image from the texture map
        bool RemoveImage(const MString& ImageName);

    private:
        std::map<MString, MImage> Textures{}; // Map to hold textures with string keys
        MString Name = "SW_Window";

    };
}