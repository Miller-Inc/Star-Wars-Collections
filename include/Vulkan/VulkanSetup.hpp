//
// Created by James Miller on 9/3/2025.
//

#pragma once
#include <string>
#include "EngineTypes/Vector.h"
#if defined(__linux__) || defined(__APPLE__)
#include <vulkan/vulkan.h>
#elif defined(_WIN32)
#include <Vulkan/vulkan.h>
#endif

namespace MillerInc::GPU
{
    class VulkanSetup
    {
    public:
        typedef struct textureImage { VkImage image{}; VkDeviceMemory memory{}; VkDescriptorSet textureId{}; MSize size; } TextureImage;
        VulkanSetup(VkInstance instance, VkDevice device, VkPhysicalDevice physical, VkQueue graphics_queue, VkCommandPool cmd_pool); // Constructor to initialize with a Vulkan device

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
        [[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;
        [[nodiscard]] TextureImage CreateTextureImage(const std::string& filename) const;

        [[nodiscard]] VkCommandBuffer BeginSingleTimeCommands() const;
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

        VkInstance instance{ VK_NULL_HANDLE };
        VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
        VkDevice device{ VK_NULL_HANDLE };
        VkQueue queue{ VK_NULL_HANDLE };
        VkCommandPool commandPool{ VK_NULL_HANDLE };
    };
}