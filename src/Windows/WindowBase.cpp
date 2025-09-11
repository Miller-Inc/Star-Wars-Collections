//
// Created by James Miller & Joshua Miller on 9/10/2025.
//

#include "Windows/WindowBase.h"
#include "EngineTypes/Logger.h"

namespace MillerInc::GUI
{
    void WindowBase::Init()
    {
        isOpen = true;
    }

    void WindowBase::Init(const MString& WindowName, MillerInc::Game::GameInstance* GameInstance)
    {
        Name = WindowName;
        isOpen = true;
        this->mGameInstance = GameInstance;
    }

    void WindowBase::Init(MillerInc::Game::GameInstance* GameInstance)
    {
        isOpen = true;
        mGameInstance = GameInstance;
    }

    void WindowBase::Open()
    {
        isOpen = true;
    }

    void WindowBase::Draw()
    {
        // Base draw does nothing
    }

    void WindowBase::Close()
    {
        isOpen = false;
    }
}