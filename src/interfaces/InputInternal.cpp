#include "vgui/IInputInternal.h"
#include <windows.h>

namespace vgui
{
    class CInputInternal : public IInputInternal
    {
    public:
        CInputInternal() : m_nFocusPanel(INVALID_PANEL), m_nMouseCapture(INVALID_PANEL) {}
        ~CInputInternal() {}
        
        void Init() override {}
        void Shutdown() override {}
        void RunFrame() override {}
        
        void OnKeyCodePressed(int code, bool isRepeat) override {}
        void OnKeyCodeReleased(int code) override {}
        void OnMousePressed(int code) override {}
        void OnMouseReleased(int code) override {}
        void OnMouseWheeled(int delta) override {}
        void OnCursorMoved(int x, int y) override {}
        void OnCursorEntered(int panel) override {}
        void OnCursorExited(int panel) override {}
        
        void SetFocus(HPanel panel) override
        {
            m_nFocusPanel = panel;
        }
        
        HPanel GetFocus() override
        {
            return m_nFocusPanel;
        }
        
        void SetMouseCapture(HPanel panel) override
        {
            m_nMouseCapture = panel;
        }
        
        HPanel GetMouseCapture() override
        {
            return m_nMouseCapture;
        }
        
    private:
        HPanel m_nFocusPanel;
        HPanel m_nMouseCapture;
    };
    
    static CInputInternal g_InputInternalInstance;
    
    void* CreateInputInternalReal()
    {
        return &g_InputInternalInstance;
    }
}
