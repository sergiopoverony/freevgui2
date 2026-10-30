#include "vgui/IInputInternal.h"
#include "vgui/Trace.h"
#include <windows.h>
#include <cstring>

namespace vgui
{
    // One object serves both "VGUI_Input004" and "VGUI_InputInternal001",
    // exactly like the original (both factories return the same pointer).
    class CInputImpl : public IInputInternal
    {
    public:
        CInputImpl() { Log("CInputImpl::CInputImpl()\n"); }

        // ---- IInput ----
        void SetMouseFocus(VPANEL newMouseFocus) override { VGUI_TRACE("IInput::SetMouseFocus(%lu)\n", newMouseFocus); m_hMouseOver = newMouseFocus; }
        void SetMouseCapture(VPANEL panel) override { VGUI_TRACE("IInput::SetMouseCapture(%lu)\n", panel); m_hCapture = panel; }
        void GetKeyCodeText(KeyCode code, char* buf, int buflen) override { if (buf && buflen > 0) buf[0] = 0; }
        VPANEL GetFocus() override { return m_hFocus; }
        VPANEL GetMouseOver() override { return m_hMouseOver; }
        void SetCursorPos(int x, int y) override { ::SetCursorPos(x, y); }
        void GetCursorPos(int& x, int& y) override { POINT pt = {0, 0}; ::GetCursorPos(&pt); x = pt.x; y = pt.y; }
        bool WasMousePressed(MouseCode code) override { return false; }
        bool WasMouseDoublePressed(MouseCode code) override { return false; }
        bool IsMouseDown(MouseCode code) override { return false; }
        void SetCursorOveride(HCursor cursor) override { m_hCursorOverride = cursor; }
        HCursor GetCursorOveride() override { return m_hCursorOverride; }
        bool WasMouseReleased(MouseCode code) override { return false; }
        bool WasKeyPressed(KeyCode code) override { return false; }
        bool IsKeyDown(KeyCode code) override { return false; }
        bool WasKeyTyped(KeyCode code) override { return false; }
        bool WasKeyReleased(KeyCode code) override { return false; }
        VPANEL GetAppModalSurface() override { return m_hModal; }
        void SetAppModalSurface(VPANEL panel) override { m_hModal = panel; }
        void ReleaseAppModalSurface() override { m_hModal = 0; }
        void GetCursorPosition(int& x, int& y) override { GetCursorPos(x, y); }

        // ---- IInputInternal ----
        void RunFrame() override {}
        void UpdateMouseFocus(int x, int y) override {}
        void PanelDeleted(VPANEL panel) override
        {
            if (m_hFocus == panel) m_hFocus = 0;
            if (m_hMouseOver == panel) m_hMouseOver = 0;
            if (m_hCapture == panel) m_hCapture = 0;
            if (m_hModal == panel) m_hModal = 0;
        }
        bool InternalCursorMoved(int x, int y) override { return false; }
        bool InternalMousePressed(MouseCode code) override { VGUI_TRACE("IInputInternal::InternalMousePressed(%d)\n", code); return false; }
        bool InternalMouseDoublePressed(MouseCode code) override { return false; }
        bool InternalMouseReleased(MouseCode code) override { return false; }
        bool InternalMouseWheeled(int delta) override { return false; }
        bool InternalKeyCodePressed(KeyCode code) override { VGUI_TRACE("IInputInternal::InternalKeyCodePressed(%d)\n", code); return false; }
        void InternalKeyCodeTyped(KeyCode code) override {}
        void InternalKeyTyped(wchar_t unichar) override {}
        bool InternalKeyCodeReleased(KeyCode code) override { return false; }
        HInputContext CreateInputContext() override { return m_nNextContext++; }
        void DestroyInputContext(HInputContext context) override {}
        void AssociatePanelWithInputContext(HInputContext context, VPANEL pRoot) override {}
        void ActivateInputContext(HInputContext context) override {}
        VPANEL GetMouseCapture() override { return m_hCapture; }
        bool IsChildOfModalPanel(VPANEL panel) override { return false; }
        void ResetInputContext(HInputContext context) override {}

    private:
        VPANEL m_hFocus = 0, m_hMouseOver = 0, m_hCapture = 0, m_hModal = 0;
        HCursor m_hCursorOverride = 0;
        HInputContext m_nNextContext = 1;
    };

    static CInputImpl g_InputInstance;

    void* CreateInputInternalReal()
    {
        Log("CreateInputInternalReal() returning %p\n", static_cast<IInputInternal*>(&g_InputInstance));
        return static_cast<IInputInternal*>(&g_InputInstance);
    }

    void* CreateInputReal()
    {
        // same pointer as InputInternal: Input004 is just the first 22 slots of it
        Log("CreateInputReal() returning %p\n", static_cast<IInputInternal*>(&g_InputInstance));
        return static_cast<IInputInternal*>(&g_InputInstance);
    }
}
