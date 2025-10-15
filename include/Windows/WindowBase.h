//
// Created by James Miller on 9/10/2025.
//

/// WindowBase.h
/// This file contains the declaration of the WindowBase class, which serves as a base class for
///     all GUI windows in the application. It provides common functionality such as initialization,
///     opening, closing, and drawing the window using ImGui. The class also manages a collection
///     of textures that can be used within the window. Derived classes can override the virtual
///     methods to implement specific behaviors for different types of windows. This class is essential
///     for creating a consistent and reusable window management system in the GUI framework. This allows
///     a developer to create new windows by inheriting from this base class and implementing
///     the necessary functionality and adding the window to the render loop in the GameInstance class.

#pragma once
#include "Game/GameInstance.h" // Forward declaration of GameInstance
#include "EngineTypes/Core.h" // Include core engine types
#include "imgui.h" // Include ImGui for GUI rendering for this class and all derived classes

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
        MMap<MString, MImage*> mTextures{}; // Map to hold textures with string keys
    public:
        WindowBase() = default;
        [[nodiscard]] MString GetName() const { return Name; }

    };
}