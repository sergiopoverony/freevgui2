#pragma once
#include "VGui.h"

namespace vgui
{
    class IInputInternal
    {
    public:
        virtual ~IInputInternal() {}
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void RunFrame() = 0;
        virtual void OnKeyCodePressed(int code, bool isRepeat) = 0;
        virtual void OnKeyCodeReleased(int code) = 0;
        virtual void OnMousePressed(int code) = 0;
        virtual void OnMouseReleased(int code) = 0;
        virtual void OnMouseWheeled(int delta) = 0;
        virtual void OnCursorMoved(int x, int y) = 0;
        virtual void OnCursorEntered(int panel) = 0;
        virtual void OnCursorExited(int panel) = 0;
        virtual void SetFocus(HPanel panel) = 0;
        virtual HPanel GetFocus() = 0;
        virtual void SetMouseCapture(HPanel panel) = 0;
        virtual HPanel GetMouseCapture() = 0;
    };
    
    #define VGUI_INPUTINTERNAL_INTERFACE_VERSION "VGUI_InputInternal001"
}
