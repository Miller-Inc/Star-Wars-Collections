//
// Created by James Miller on 9/10/2025.
//

#pragma once
#include "EngineTypes/String.h"
#include "EngineTypes/Image.h"
#include <map>
#include "Array.h"

namespace MillerInc
{

    typedef struct mImageLoadData
    {
        MString Path;
        MString Name;
        MVector2 Position;
        MVector2 Scale;
        MVector2 Size;
        std::vector<MString> WindowTags; // Tags for categorizing or grouping images
    } MImageLoadData;

    class ResourceLoader
    {
    public:
        ResourceLoader() = default;
        ~ResourceLoader() = default;

        std::map<MString, MImageLoadData> ImageData{}; // Map to hold image load data with string keys
        bool LoadResources(const MString& FilePath);
    };
}