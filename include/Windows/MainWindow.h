//
// Created by James Miller & Joshua Miller on 9/10/2025.
//

#pragma once

#include <queue>
#include <stack>

#include "WindowBase.h"
#include "Game/WindowTypes.h"

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