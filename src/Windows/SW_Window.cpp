//
// Created by James Miller on 9/2/2025.
//

#include <Windows/SW_Window.h>
#include "imgui.h"
#include <stdexcept>
#include "EngineTypes/Logger.h"
#include "Game/GameInstance.h"

namespace MillerInc::GUI
{
    SW_Window::SW_Window(const SW_Window& other)
    {
        Name = other.Name;
        Textures = other.Textures;
    }

    void SW_Window::Init()
    {
        Textures.clear();
        if (mGameInstance)
        {
            mGameInstance->OpenImage("Resources/Textures/palm_trees.png",
                "Palm Tree", {5, 50}, {0.05f, 0.05f});
            if (MImage* image = mGameInstance->GetImage("Palm Tree"))
            {
                AddImage(image);
            }
        }
        else
        {
            M_LOGGER(Logger::LogCore, Logger::Warning, "GameInstance is null. Cannot load default images.");
        }
    }

    void SW_Window::Init(const MString& WindowName, MillerInc::Game::GameInstance* GameInstance)
    {
        Name = WindowName;
        mGameInstance = GameInstance;
        Init();
    }

    void SW_Window::Init(MillerInc::Game::GameInstance* GameInstance)
    {
        Name = "SW_Window";
        mGameInstance = GameInstance;
        Init();
    }

    void SW_Window::TestWindow()
    {
        if (ImGui::Begin(Name.c_str()))
        {
            // Images are last due to cursor movement
            for (const auto& [name, image] : Textures)
            {
                ImGui::SetCursorPos({image->Position.x, image->Position.y});
                ImGui::Text("Image Name: %s", name.c_str());
                ImGui::Image(image->TextureHandle.textureId, ImVec2(image->Size.x * image->Scale.x, image->Size.y * image->Scale.y));
                ImGui::Separator();
            }
            ImGui::End();
        }
    }

    void SW_Window::AddImage(MImage* image)
    {
        if (!image)
        {
            M_LOGGER(Logger::LogCore, Logger::Error, "Image is null.");
            return;
        }

        if (!Textures.contains(image->Name))
        {
            Textures.emplace(image->Name, image);
            return;
        }
        else
        {
            M_LOGGER(Logger::LogCore, Logger::Warning, "Image with name " + image->Name + " already exists in window " + Name + ". Use a different name.");
            return;
        }
    }

    MImage* SW_Window::RemoveImage(const MString& ImageName)
    {
        MImage* image = Textures[ImageName];
        Textures.erase(ImageName);
        return image;
    }
}




