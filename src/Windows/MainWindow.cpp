//
// Created by James Miller & Joshua on 9/10/2025.
//

#include "Windows/MainWindow.h"
#include "Windows/NetworkPlayWindow.h"
#include "EngineTypes/Logger.h"
#include "EngineTypes/InputValidation.h"

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
            case Game::WindowType::NetworkWindow:
                OpenNetworkWindow();
                break;
            case Game::WindowType::NetworkJoinSetupWindow:
                JoinGameWindow();
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
        M_LOGGER(Logger::LogCore, Logger::Warning, "Opening Story mode.");
    }

    void MainWindow::OpenFreePlay()
    {
    }

    void MainWindow::OpenMultiplayer()
    {
        Type = Game::WindowType::NetworkWindow;
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

    void MainWindow::OpenNetworkWindow()
    {
        ImGui::SetCursorPos(ImVec2(590,275));
        if (ImGui::Button("Host Game", ImVec2(100,50)))
        {
            Type = Game::WindowType::NetworkHostSetupWindow;
        }

        ImGui::SetCursorPos(ImVec2(590,345));
        if (ImGui::Button("Join Game", ImVec2(100,50)))
        {
            Type = Game::WindowType::NetworkJoinSetupWindow;
        }
    }

    void MainWindow::JoinGameWindow()
    {
        ImGui::SetCursorPos(ImVec2(550,210));
        ImGui::PushItemWidth(200); //NOTE: (Push/Pop)ItemWidth is optional
        static char ip_string[128] = "";
        ImGui::InputText("##IP_Input", ip_string, IM_ARRAYSIZE(ip_string));
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(600,190));
        ImGui::Text("Remote Host");

        ImGui::SetCursorPos(ImVec2(620,260));
        ImGui::Text("Port");

        ImGui::SetCursorPos(ImVec2(550,280));
        ImGui::PushItemWidth(200);
        static int port = 123;
        ImGui::InputInt("##PortInput", &port);
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(590,350));
        if (ImGui::Button("Test Connection", ImVec2(115,25)))
        {
            if (InputValidation::IsValidIP(ip_string))
            {
                M_LOGGER(Logger::LogNetwork, Logger::Info, "Valid IP Address: " + std::string(ip_string));
                bNetworkConnecting = true;
                if (mGameInstance)
                {
                    if (mGameInstance->ConnectToServer("Multiplayer_Server", std::string(ip_string), port))
                    {
                        Type = Game::WindowType::LoadingWindow; // TODO: Change to actual multiplayer window when ready
                    }
                }
            } else
            {
                M_LOGGER(Logger::LogNetwork, Logger::Warning, "Invalid IP Address: " + std::string(ip_string));
            }
        }

        if (bNetworkConnecting)
        {
            ImGui::SetCursorPos(ImVec2(302,453.5));
            ImGui::PushItemWidth(200);
            static float connection_progress = 0.0f;
            ImGui::ProgressBar(connection_progress, ImVec2(0.0f, 0.0f));
            ImGui::PopItemWidth();
        }
    }

}
