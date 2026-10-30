#include "vgui/IInput.h"
#include <windows.h>

namespace vgui
{
    extern void Log(const char* format, ...);
    
    class CInput : public IInput
    {
    public:
        CInput() : m_nFocusPanel(INVALID_PANEL), m_nMouseCapture(INVALID_PANEL) 
        {
            Log("CInput::CInput()\n");
            for (int i = 0; i < 256; i++)
            {
                m_bKeyDown[i] = false;
                m_bMouseDown[i] = false;
            }
        }
        
        void Init() override { Log("IInput::Init()\n"); }
        void Shutdown() override { Log("IInput::Shutdown()\n"); }
        
        void SetFocus(HPanel panel) override { Log("IInput::SetFocus(%u)\n", panel); m_nFocusPanel = panel; }
        HPanel GetFocus() override { Log("IInput::GetFocus()\n"); return m_nFocusPanel; }
        
        void SetMouseCapture(HPanel panel) override { Log("IInput::SetMouseCapture(%u)\n", panel); m_nMouseCapture = panel; }
        HPanel GetMouseCapture() override { Log("IInput::GetMouseCapture()\n"); return m_nMouseCapture; }
        
        bool IsMouseDown(int code) override { Log("IInput::IsMouseDown(%d)\n", code); return (code >= 0 && code < 256) ? m_bMouseDown[code] : false; }
        bool IsKeyDown(int code) override { Log("IInput::IsKeyDown(%d)\n", code); return (code >= 0 && code < 256) ? m_bKeyDown[code] : false; }
        
        void GetCursorPos(int& x, int& y) override 
        { 
            Log("IInput::GetCursorPos()\n");
            POINT pt;
            ::GetCursorPos(&pt);
            x = pt.x;
            y = pt.y;
        }
        
        void SetCursorPos(int x, int y) override
        {
            Log("IInput::SetCursorPos(%d, %d)\n", x, y);
            ::SetCursorPos(x, y);
        }
        
        void OnKeyDown(int keyCode) { if (keyCode >= 0 && keyCode < 256) m_bKeyDown[keyCode] = true; }
        void OnKeyUp(int keyCode) { if (keyCode >= 0 && keyCode < 256) m_bKeyDown[keyCode] = false; }
        void OnMouseDown(int button) { if (button >= 0 && button < 256) m_bMouseDown[button] = true; }
        void OnMouseUp(int button) { if (button >= 0 && button < 256) m_bMouseDown[button] = false; }
        
    private:
        HPanel m_nFocusPanel;
        HPanel m_nMouseCapture;
        bool m_bKeyDown[256];
        bool m_bMouseDown[256];
    };
    
    static CInput g_InputInstance;
    
    void* CreateInputReal()
    {
        Log("CreateInputReal() returning %p\n", &g_InputInstance);
        return &g_InputInstance;
    }
}
