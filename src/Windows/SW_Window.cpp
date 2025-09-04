//
// Created by James Miller on 9/2/2025.
//

#include <Windows/SW_Window.h>
#include "imgui.h"
#include <stdexcept>
#include "EngineTypes/Logger.h"

namespace MillerInc::GUI
{
    SW_Window::SW_Window(const SW_Window& other)
    {
        Setup = other.Setup;
    }

    SW_Window::SW_Window(GPU::VulkanSetup* setup)
    {
        Setup = setup;
    }

    void SW_Window::Init()
    {
        Textures.clear();

    }

    void SW_Window::Init(const MString& WindowName)
    {
        Name = WindowName;
        Init();
    }

    void SW_Window::TestWindow() const
    {
        ImGui::Begin(Name.c_str());
        // Images are last due to cursor movement
        for (const auto& [name, image] : Textures)
        {
            ImGui::SetCursorPos({image.Position.x, image.Position.y});
            ImGui::Text("Image Name: %s", name.c_str());
            ImGui::Image(image.TextureHandle.textureId, ImVec2(image.Size.x, image.Size.y));
        }
        ImGui::End();
    }

    bool SW_Window::AddImage(const MString& PathToImage, const MString& ImageName, const MVector2& Position)
    {
        const MillerInc::GPU::VulkanSetup::TextureImage texture = Setup->CreateTextureImage(PathToImage);
        if (texture.image == VK_NULL_HANDLE)
        {
            M_LOGGER(Logger::LogGraphics, Logger::Error, "Failed to load texture image from path: " + PathToImage);
            return false;
        }
        MImage newImage = {texture, ImageName, texture.size, Position};
        Textures.emplace(ImageName, newImage);
        return true;
    }

    MImage* SW_Window::GetImage(const MString& ImageName)
    {
        return &Textures[ImageName];
    }

    bool SW_Window::RemoveImage(const MString& ImageName)
    {
        MImage* image = GetImage(ImageName);
        Textures.erase(ImageName);
        return image != nullptr;
    }
}




