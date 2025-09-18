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
        isOpen = false;
    }

    MainWindow::MainWindow(const MainWindow& other) : WindowBase(other)
    {
        Name = other.Name;
        isOpen = other.isOpen;
    }

    void MainWindow::Open()
    {
        WindowBase::Open();
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
                OpeningWindow();
                break;
            case Game::WindowType::SelectionWindow:
                SelectionWindow();
                break;
            case Game::WindowType::LoadingWindow:
                LoadingWindow();
                break;
            default:
                OpeningWindow();
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

    void MainWindow::OpeningWindow()
    {
        if (Textures.contains("Opening Image"))
        {
            const auto& image = Textures["Opening Image"];
            ImGui::Image(image->TextureHandle.textureId, {image->Size.x, image->Size.y});
        }

        ImGui::SetCursorPos(ImVec2(515,680));
        if (ImGui::Button("Press/Tap to Start", ImVec2(250,50)))
        {
            Type = Game::WindowType::SelectionWindow;
        }
    }

    void MainWindow::SelectionWindow()
    {
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
    }

    void MainWindow::LoadingWindow()
    {
        if (Textures.contains("Stars"))
        {
            ImGui::SetCursorPos(ImVec2(0,0));
            const auto& image = Textures["Stars"];
            ImGui::Image(image->TextureHandle.textureId, {image->Size.x, image->Size.y - 8});
        }
    }

}
