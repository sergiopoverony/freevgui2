#pragma once
#include "VGui.h"

namespace vgui
{
    class IInput
    {
    public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void SetFocus(HPanel panel) = 0;
        virtual HPanel GetFocus() = 0;
        virtual void SetMouseCapture(HPanel panel) = 0;
        virtual HPanel GetMouseCapture() = 0;
        virtual bool IsMouseDown(int code) = 0;
        virtual bool IsKeyDown(int code) = 0;
        virtual void GetCursorPos(int& x, int& y) = 0;
        virtual void SetCursorPos(int x, int y) = 0;
    };
    
    #define VGUI_INPUT_INTERFACE_VERSION "VGUI_Input004"
}