//
// Created by James Miller & Joshua on 9/10/2025.
//

#include "Windows/MainWindow.h"
#include "Windows/NetworkPlayWindow.h"
#include "EngineTypes/Logger.h"
#include "EngineTypes/InputValidation.h"
#include "Game/MacroDefs.h"

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

        if (ImGui::Begin("Star Wars: Card Game"), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)
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
            case Game::WindowType::NetworkHostSetupWindow:
                HostWaitingWindow();
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

    void MainWindow::SetOpenWindowType(const Game::WindowType NewWindow)
    {
        // If the new window is the opening window, clear the stack
        if (NewWindow == Game::WindowType::OpeningWindow)
        {
            Type = Game::WindowType::OpeningWindow;
            while (!WindowStack.empty())
            {
                WindowStack.pop();
            }
            return;
        }

        // If the new window is the same as the current window, do nothing
        if (Type != NewWindow)
        {
            WindowStack.push(Type);
            Type = NewWindow;
        }
    }

    void MainWindow::GoBack()
    {
        if (!WindowStack.empty())
        {
            Type = WindowStack.top();
            WindowStack.pop();
            while (Type == Game::WindowType::LoadingWindow && !WindowStack.empty())
            {
                if (!WindowStack.empty())
                {
                    Type = WindowStack.top();
                    WindowStack.pop();
                } else
                {
                    Type = Game::WindowType::OpeningWindow;
                }
            }
        } else
        {
            Type = Game::WindowType::OpeningWindow;
        }
    }

    void MainWindow::OpenSettings()
    {
        M_LOGGER(Logger::LogCore, Logger::Warning, "Opening Settings.");
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
        SetOpenWindowType(Game::WindowType::NetworkWindow);
    }

    void MainWindow::ExitGame()
    {
        SetOpenWindowType(Game::WindowType::OpeningWindow);
        if (mGameInstance)
        {
            mGameInstance->StopMainLoop();
        }
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
            SetOpenWindowType(Game::WindowType::SelectionWindow);
        }
    }

    void MainWindow::SelectionWindow()
    {
        AddBackButton();

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
        AddBackButton();

        ImGui::SetCursorPos(ImVec2(590,275));
        if (ImGui::Button("Host Game", ImVec2(100,50)))
        {
            SetOpenWindowType(Game::WindowType::NetworkHostSetupWindow);
            SetOpenWindowType(Game::WindowType::LoadingWindow); // Temporary until multiplayer is ready
            StartGameServer(); // Start server asynchronously and go to loading screen
        }

        ImGui::SetCursorPos(ImVec2(590,345));
        if (ImGui::Button("Join Game", ImVec2(100,50)))
        {
            SetOpenWindowType(Game::WindowType::NetworkJoinSetupWindow);
        }
    }

    void MainWindow::JoinGameWindow()
    {
        AddBackButton();
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
                        SetOpenWindowType(Game::WindowType::LoadingWindow); // TODO: Change to actual multiplayer window when ready
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

    void MainWindow::HostWaitingWindow()
    {
        AddBackButton();

        ImGui::SetCursorPos(ImVec2(524.5,149.5));
        ImGui::Text("Connect with Following Addresses:\n%s", mServerIpsString.c_str()); // Display all server IPs

        ImGui::SetCursorPos(ImVec2(580.5,330.5));
        ImGui::Text("Connected Users:\n%s", mConnectedClientsString.c_str()); // Display all connected clients

        ImGui::SetCursorPos(ImVec2(1065,32));
        if (ImGui::Button("Continue With Current Users", ImVec2(200,50)))
        {

        }
    }

    void MainWindow::AddBackButton()
    {
        ImGui::SetCursorPos(ImVec2(5,25));
        if (ImGui::Button("Back", ImVec2(100,50)))
        {
            GoBack();
        }
    }

    void MainWindow::StartGameServer()
    {
        const auto serverIps = mGameInstance->StartServerAsync(DEFAULT_PORT, [this](int32_t port, const MString& clientIp, SocketPtr clientSocket)
        {
            mConnectedClients += clientSocket;
            mConnectedClientsString += clientIp + "\n";
        });
        for (auto& serverIp : serverIps)
        {
            M_LOGGER(Logger::LogTemp, Logger::Info, "Server IP: " + serverIp);
        }

        if (serverIps.IsEmpty())
        {
            M_LOGGER(Logger::LogNetwork, Logger::Error, "Failed to start game server.");
            GoBack(); // Go back to previous window if server failed to start
            return;
        }

        mServerIps = serverIps;
        mServerIpsString = ""; // Clear previous string
        for (const auto& ip : mServerIps)
        {
            mServerIpsString += ip + "\n";
        }

        GoBack();
    }
}
