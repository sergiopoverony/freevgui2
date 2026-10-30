#pragma once
#include "VGui.h"

namespace vgui
{
    typedef int KeyCode;
    typedef int MouseCode;

    // VGUI_Input004: slots 0..21 of the object shared with VGUI_InputInternal001.
    // Verified against the original (original/vtable_signatures.txt).
    class IInput
    {
    public:
        virtual ~IInput() {}                                                          // 0
        virtual void SetMouseFocus(VPANEL newMouseFocus) = 0;                         // 1
        virtual void SetMouseCapture(VPANEL panel) = 0;                               // 2
        virtual void GetKeyCodeText(KeyCode code, char* buf, int buflen) = 0;         // 3
        virtual VPANEL GetFocus() = 0;                                                // 4
        virtual VPANEL GetMouseOver() = 0;                                            // 5
        virtual void SetCursorPos(int x, int y) = 0;                                  // 6
        virtual void GetCursorPos(int& x, int& y) = 0;                                // 7
        virtual bool WasMousePressed(MouseCode code) = 0;                             // 8
        virtual bool WasMouseDoublePressed(MouseCode code) = 0;                       // 9
        virtual bool IsMouseDown(MouseCode code) = 0;                                 // 10
        virtual void SetCursorOveride(HCursor cursor) = 0;                            // 11
        virtual HCursor GetCursorOveride() = 0;                                       // 12
        virtual bool WasMouseReleased(MouseCode code) = 0;                            // 13
        virtual bool WasKeyPressed(KeyCode code) = 0;                                 // 14
        virtual bool IsKeyDown(KeyCode code) = 0;                                     // 15
        virtual bool WasKeyTyped(KeyCode code) = 0;                                   // 16
        virtual bool WasKeyReleased(KeyCode code) = 0;                                // 17
        virtual VPANEL GetAppModalSurface() = 0;                                      // 18
        virtual void SetAppModalSurface(VPANEL panel) = 0;                            // 19
        virtual void ReleaseAppModalSurface() = 0;                                    // 20
        virtual void GetCursorPosition(int& x, int& y) = 0;                           // 21
    };

    #define VGUI_INPUT_INTERFACE_VERSION "VGUI_Input004"
}
