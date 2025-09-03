//
// Created by James Miller on 9/2/2025.
//

#include <Windows/SW_Window.h>
#include "imgui_impl_vulkan.h"
#include "stb_image.h"
#include <stdexcept>
#include <cstring>
#include <iostream>
extern uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
extern void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

void SW_Window::Init(const VkDevice &device)
{
    // 1. Load image data using stb_image
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("path/to/your/image.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels)
    {
        // Handle error
        return;
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (Setup)
    {
        const VulkanSetup::TextureImage image = Setup->CreateTextureImage("image"); // Ensure this path is correct
        VkImage vulkanImage = image.image;
        VkDeviceMemory devMem = image.memory;

    }




    TexWidth = texWidth;
    TexHeight = texHeight;

}

void SW_Window::TestWindow() const
{
    ImGui::Image(TextureID, ImVec2((float)TexWidth, (float)TexHeight));
}





