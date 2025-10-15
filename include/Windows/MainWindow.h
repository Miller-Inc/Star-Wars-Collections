//
// Created by James Miller & Joshua Miller on 9/10/2025.
//

/// MainWindow.h
/// This file contains the declaration of the MainWindow class, which manages the main menu and navigation
///     between different game modes and settings. It utilizes ImGui for rendering the user interface and
///     handles user interactions to transition between various windows such as story mode, free play,
///     multiplayer, training, and settings. The MainWindow class also manages network connections for
///     multiplayer functionality, including hosting and joining games. It maintains a stack of window types
///     to allow users to navigate back to previous windows seamlessly. This class is a crucial part of the
///     game's user interface, providing an intuitive and interactive experience for players.

#pragma once

#include <stack> // For managing window history
#include "WindowBase.h" // Base class for all windows
#include "Game/WindowTypes.h" // Enum for different window types

namespace MillerInc::GUI
{

    class MainWindow : public WindowBase
    {
    public:
        MainWindow();
        MainWindow(const MainWindow& other);
        ~MainWindow() override = default;

        void Open() override;
        void Draw() override;
        void Close() override;

        void SetOpenWindowType(Game::WindowType NewWindow);
        void GoBack();


    protected:
        Game::WindowType Type = Game::WindowType::OpeningWindow;
        std::stack<Game::WindowType> WindowStack;

        void OpenSettings();
        void OpenTraining();
        void OpenStoryMode();
        void OpenFreePlay();
        void OpenMultiplayer();
        void ExitGame();

    private:
        void SelectionWindow();
        void OpeningWindow();
        void LoadingWindow();
        void OpenNetworkWindow();
        void JoinGameWindow();
        void HostWaitingWindow();

        void AddBackButton();

        bool bNetworkWindowOpened = true;
        bool bNetworkConnecting = false;

        void StartGameServer();
        MArray<MString> mServerIps;
        MString mServerIpsString;
        MString mConnectedClientsString;
        MArray<SocketPtr> mConnectedClients;

    };
}