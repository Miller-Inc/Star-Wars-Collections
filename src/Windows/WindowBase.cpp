//
// Created by James Miller & Joshua Miller on 9/10/2025.
//

#include "Windows/WindowBase.h"

namespace MillerInc::GUI
{
    void WindowBase::Init()
    {
        isOpen = true;

        if (mGameInstance != nullptr)
        {
            mTextures = mGameInstance->LoadResources(Name);
        } else {
            M_LOGGER(Logger::LogCore, Logger::Warning, "GameInstance is null. Cannot load resources for window: " + Name);
        }

        Open(); // Open the window by default after initialization
    }

    void WindowBase::Init(const MString& WindowName, MillerInc::Game::GameInstance* GameInstance)
    {
        Name = WindowName;
        this->mGameInstance = GameInstance;
        Init();
    }

    void WindowBase::Init(MillerInc::Game::GameInstance* GameInstance)
    {
        mGameInstance = GameInstance;
        Init();
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