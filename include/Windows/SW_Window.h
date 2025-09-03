//
// Created by James Miller on 9/2/2025.
//

#pragma once
#include "SDL3/SDL.h"
#include "imgui.h"
#include <Vulkan/vulkan.h>
#include "Vulkan/VulkanSetup.h"

class SW_Window
{
public:
    void Init(const VkDevice& device); // Initialize ImGui context and SDL window
    void TestWindow() const; // Simple test window
    VulkanSetup* Setup = nullptr;
private:
    VkDescriptorSet TextureID{};
    int TexWidth{}, TexHeight{};
};

