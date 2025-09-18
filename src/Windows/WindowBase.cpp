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
            M_LOGGER(Logger::LogCore, Logger::Info, "Loading resources for window: " + Name);
            mGameInstance->LoadResources(Name);
        }
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