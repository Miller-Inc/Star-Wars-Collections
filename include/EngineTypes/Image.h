//
// Created by James Miller on 9/3/2025.
//

#pragma once

#include "EngineTypes/Vector.h"
#include "Vulkan/VulkanSetup.hpp"

namespace MillerInc
{
    typedef struct mImage
    {
        MillerInc::GPU::VulkanSetup::TextureImage TextureHandle;
        std::string Name;
        MillerInc::MSize Size{}; // Size of the image
        MillerInc::MVector2 Position{}; // Position of the image
    } MImage;
}