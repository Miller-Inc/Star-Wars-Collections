//
// Created by James Miller & Joshua Miller on 9/10/2025.
//

#pragma once

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

    protected:
        Game::WindowType Type = Game::WindowType::OpeningWindow;

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

    };
}