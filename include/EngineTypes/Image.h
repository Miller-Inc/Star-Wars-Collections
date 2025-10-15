//
// Created by James Miller on 9/3/2025.
//

#pragma once

#include "EngineTypes/Vector.h"
#include "Vulkan/VulkanSetup.hpp"
#include <cstdint>

namespace MillerInc
{
    typedef struct mImage
    {
        GPU::VulkanSetup::TextureImage TextureHandle;
        MString Name;
        MillerInc::MSize Size{}; // Size of the image
        MillerInc::MVector2 Position{}; // Position of the image
        MillerInc::MVector2 Scale{ 1.0f, 1.0f}; // Scale of the image
        int32_t ref_count = 0; // Reference count for shared usage
    } MImage;
}