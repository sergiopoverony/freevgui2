#pragma once
#include "IInput.h"

namespace vgui
{
    typedef int HInputContext;

    // VGUI_InputInternal001: IInput + slots 22..40 (41 slots total).
    // Argument counts verified against the original; names follow the Source SDK.
    class IInputInternal : public IInput
    {
    public:
        virtual void RunFrame() = 0;                                                  // 22
        virtual void UpdateMouseFocus(int x, int y) = 0;                              // 23
        virtual void PanelDeleted(VPANEL panel) = 0;                                  // 24
        virtual bool InternalCursorMoved(int x, int y) = 0;                           // 25
        virtual bool InternalMousePressed(MouseCode code) = 0;                        // 26
        virtual bool InternalMouseDoublePressed(MouseCode code) = 0;                  // 27
        virtual bool InternalMouseReleased(MouseCode code) = 0;                       // 28
        virtual bool InternalMouseWheeled(int delta) = 0;                             // 29
        virtual bool InternalKeyCodePressed(KeyCode code) = 0;                        // 30
        virtual void InternalKeyCodeTyped(KeyCode code) = 0;                          // 31
        virtual void InternalKeyTyped(wchar_t unichar) = 0;                           // 32
        virtual bool InternalKeyCodeReleased(KeyCode code) = 0;                       // 33
        virtual HInputContext CreateInputContext() = 0;                               // 34
        virtual void DestroyInputContext(HInputContext context) = 0;                  // 35
        virtual void AssociatePanelWithInputContext(HInputContext context, VPANEL pRoot) = 0; // 36
        virtual void ActivateInputContext(HInputContext context) = 0;                 // 37
        virtual VPANEL GetMouseCapture() = 0;                                         // 38
        virtual bool IsChildOfModalPanel(VPANEL panel) = 0;                           // 39
        virtual void ResetInputContext(HInputContext context) = 0;                    // 40
    };

    #define VGUI_INPUTINTERNAL_INTERFACE_VERSION "VGUI_InputInternal001"
}
