// Generated with ImRAD 0.8
// visit https://github.com/tpecholt/imrad

#pragma once
#include <string>
namespace MillerInc::GUI
{
    class NetworkPlayWindow
    {
    public:
        /// @begin interface
        void Open();
        void Close();
        void Draw();

        std::string ipAddress;
        int port;
        /// @end interface

    private:
        /// @begin impl
        void TryConnect();

        bool isOpen = true;
        /// @end impl
    };
}