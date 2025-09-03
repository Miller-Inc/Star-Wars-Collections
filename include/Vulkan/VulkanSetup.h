//
// Created by James Miller on 9/3/2025.
//

#pragma once
#include <string>

#include "Vulkan/vulkan.h"

class VulkanSetup
{
public:
    typedef struct { VkImage image; VkDeviceMemory memory;  } TextureImage;
    VulkanSetup(VkInstance instance, VkDevice device, VkPhysicalDevice physical); // Constructor to initialize with a Vulkan device

    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
    [[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;
    [[nodiscard]] TextureImage CreateTextureImage(const std::string& filename) const;

    VkInstance instance{ VK_NULL_HANDLE };
    VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
    VkDevice device{ VK_NULL_HANDLE };
};