//
// Created by James Miller & Joshua on 9/10/2025.
//

#include "Windows/MainWindow.h"
#include "Windows/NetworkPlayWindow.h"
#include "EngineTypes/Logger.h"

namespace MillerInc::GUI
{
    MainWindow::MainWindow() : WindowBase()
    {
        Name = "Main Window";
        isOpen = true;
    }

    MainWindow::MainWindow(const MainWindow& other) : WindowBase(other)
    {
        Name = other.Name;
        isOpen = other.isOpen;
    }

    void MainWindow::Init()
    {
        WindowBase::Init();
        Open();
    }

    void MainWindow::Init(const MString& WindowName, MillerInc::Game::GameInstance* GameInstance)
    {
        WindowBase::Init(WindowName, GameInstance);
        Open();
    }

    void MainWindow::Init(MillerInc::Game::GameInstance* GameInstance)
    {
        WindowBase::Init(GameInstance);
        Open();
    }

    void MainWindow::Open()
    {
        WindowBase::Open();
        if (mGameInstance)
        {
            MImage* opening = mGameInstance->GetImage("Opening Image");
            if (opening)
            {
                Textures.emplace("Opening Image", opening);
            } else
            {
                M_LOGGER(Logger::LogCore, Logger::Warning, "Failed to load 'opening.png'.");
            }
        }
        else
        {
            M_LOGGER(Logger::LogCore, Logger::Warning, "GameInstance is null. Cannot load default images.");
        }
    }

    void MainWindow::Draw()
    {
        WindowBase::Draw();

        static bool window = true;
        ImGui::SetNextWindowSize(ImVec2(1280,760));

        if (ImGui::Begin("Star Wars: Card Game", &window), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)
        {
            switch (Type)
            {

            case Game::WindowType::OpeningWindow:
                if (Textures.contains("Opening Image"))
                {
                    auto image = Textures["Opening Image"];
                    ImGui::Image(image->TextureHandle.textureId, {image->Size.x, image->Size.y});
                }

                ImGui::SetCursorPos(ImVec2(515,680));
                if (ImGui::Button("Press/Tap to Start", ImVec2(250,50)))
                {
                    Type = Game::WindowType::SelectionWindow;
                }
                break;
            case Game::WindowType::SelectionWindow:
                ImGui::SetCursorPos(ImVec2(590,100));
                if (ImGui::Button("Story Mode", ImVec2(100,50)))
                {
                    OpenStoryMode();
                }

                ImGui::SetCursorPos(ImVec2(590,200));
                if (ImGui::Button("Free Play", ImVec2(100,50)))
                {
                    OpenFreePlay();
                }

                ImGui::SetCursorPos(ImVec2(590,300));
                if (ImGui::Button("Multiplayer", ImVec2(100,50)))
                {
                    OpenMultiplayer();
                }

                ImGui::SetCursorPos(ImVec2(590,400));
                if (ImGui::Button("Training", ImVec2(100,50)))
                {
                    OpenTraining();
                }

                ImGui::SetCursorPos(ImVec2(590,500));
                if (ImGui::Button("Settings", ImVec2(100,50)))
                {
                    OpenSettings();
                }

                ImGui::SetCursorPos(ImVec2(590,600));
                if (ImGui::Button("Exit Game", ImVec2(100,50)))
                {
                    ExitGame();
                }
                break;
            default:
                    break;
            }

            ImGui::End();
        }

    }

    void MainWindow::Close()
    {
        WindowBase::Close();
        if (mGameInstance)
        {
            for (const auto& [name, image] : Textures)
            {
                if (image)
                {
                    mGameInstance->RemoveImage(name);
                }
            }
            Textures.clear();
        } else
        {
            M_LOGGER(Logger::LogCore, Logger::Warning, "GameInstance is null. Cannot unload images.");
        }
    }

    void MainWindow::OpenSettings()
    {

    }

    void MainWindow::OpenTraining()
    {
    }

    void MainWindow::OpenStoryMode()
    {
    }

    void MainWindow::OpenFreePlay()
    {
    }

    void MainWindow::OpenMultiplayer()
    {

    }

    void MainWindow::ExitGame()
    {
        Type = Game::WindowType::OpeningWindow;
    }
}
