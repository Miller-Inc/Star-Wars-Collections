#include "Windows/NetworkPlayWindow.h"
#include "EngineTypes/Logger.h"
#include "imgui.h"

namespace MillerInc::GUI
{
    NetworkPlayWindow networkPlayWindow;


    void NetworkPlayWindow::Open()
    {
        isOpen = true;
    }

    void NetworkPlayWindow::Close()
    {
        isOpen = false;
    }

    void NetworkPlayWindow::Draw()
    {
        /// @style Dark
        /// @unit px
        /// @begin TopWindow
        ImGui::SetNextWindowSize({ 494, 343 }, ImGuiCond_Always); //{ 300, 250 }
        if (isOpen && ImGui::Begin("Network Play###NetworkPlayWindow", &isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
        {

            ImGui::SetCursorPos(ImVec2(191,114.5));
            ImGui::Text("Enter IP Address");

            ImGui::SetCursorPos(ImVec2(147,144.5));
            ImGui::PushItemWidth(200); //NOTE: (Push/Pop)ItemWidth is optional
            static char ipBuffer[128] = "";
            ImGui::InputText("##IPAddress", ipBuffer, IM_ARRAYSIZE(ipBuffer));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(147,173.875));
            ImGui::PushItemWidth(200);
            static int portNum = 123;
            ImGui::InputInt("##portInput", &portNum);
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(214.5,209.5));
            if (ImGui::Button("Connect", ImVec2(57,19)))
            {
                ipAddress = ipBuffer;
                port = portNum;
                TryConnect();
            }//remove size argument (ImVec2) to auto-resize
            ImGui::End();
        }
        /// @end TopWindow
    }

    void NetworkPlayWindow::TryConnect()
    {
        M_LOGGER(Logger::LogCore, Logger::Info, "Attempting to connect to " + ipAddress + ":" + std::to_string(port));
        // Implement connection logic here
    }
}

