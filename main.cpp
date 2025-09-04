#include <functional>

#include "EngineTypes/Logger.h"
#include "Game/GameInstance.h"
#include "Windows/SW_Window.h"


// Main code
int main(int argc, char** argv)
{
    MillerInc::Game::GameInstance gameInstance{};
    MillerInc::GUI::SW_Window window{};
    bool show_main_window = true;

    MillerInc::Game::MWindow mainWindow
    {
        "Main Window",
        {[&window]()
        {
            // M_LOGGER(MillerInc::Logger::LogTemp, MillerInc::Logger::Info, "Drawing Main Window");
            window.TestWindow();
        }},
        {[&window](MillerInc::Game::GameInstance* GameInstance)
        {
            window.Init(GameInstance);
        }},
        nullptr,
        &show_main_window
    };

    gameInstance.AddWindow(mainWindow);

    MillerInc::GUI::SW_Window window2{};
    bool show_main_window2 = true;

    MillerInc::Game::MWindow mainWindow2
    {
        "Main Window 2",
        {[&window2]()
        {
            // M_LOGGER(MillerInc::Logger::LogTemp, MillerInc::Logger::Info, "Drawing Main Window");
            window2.TestWindow();
        }},
        {[&window2](MillerInc::Game::GameInstance* GameInstance)
        {
            window2.Init("Main Window 2", GameInstance);
            window2.RemoveImage("Palm Tree");
            window2.AddImage(GameInstance->OpenGetImage("Resources/Textures/image.png", "Image", {30, 20}, {0.5f, 0.5f}));
        }},
        nullptr,
        &show_main_window
    };

    gameInstance.AddWindow(mainWindow2);

    gameInstance.Init();

    return 0;
}
