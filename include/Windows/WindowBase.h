//
// Created by jwmil on 9/10/2025.
//

#pragma once
#include "Game/GameInstance.h"
#include "imgui.h"

namespace MillerInc::GUI
{
    class WindowBase
    {
    public:
        virtual void Init();
        virtual void Init(const MString& WindowName, MillerInc::Game::GameInstance* GameInstance);
        virtual void Init(MillerInc::Game::GameInstance* GameInstance);
        WindowBase(const WindowBase& other)
        {
            Name = other.Name;
            isOpen = other.isOpen;
        }
        virtual ~WindowBase() = default;
        virtual void Open();
        virtual void Draw();
        virtual void Close();
        bool isOpen = false;
        MString Name = "WindowBase";
    protected:
        MillerInc::Game::GameInstance* mGameInstance = nullptr;
        std::map<MString, MImage*> Textures{}; // Map to hold textures with string keys
    public:
        WindowBase() = default;
        [[nodiscard]] MString GetName() const { return Name; }

    };
}