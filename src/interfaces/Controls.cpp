#include "vgui/VPanel.h"
#include "vgui/ISurface.h"
#include <windows.h>
#include <string>

namespace vgui
{
    extern void* CreateSurfaceReal();
    
    class CBasePanel : public VPanel
    {
    public:
        CBasePanel() : m_bPaintEnabled(true) {}
        virtual ~CBasePanel() {}
        
        virtual void Paint()
        {
            if (!m_bPaintEnabled) return;
            
            ISurface* pSurface = (ISurface*)CreateSurfaceReal();
            if (!pSurface) return;
            
            int x, y, wide, tall;
            GetPos(x, y);
            GetSize(wide, tall);
            
            pSurface->DrawSetColor(200, 200, 200, 255);
            pSurface->DrawFilledRect(x, y, x + wide, y + tall);
            
            pSurface->DrawSetColor(100, 100, 100, 255);
            pSurface->DrawOutlinedRect(x, y, x + wide, y + tall);
        }
        
        void SetPaintEnabled(bool enabled) { m_bPaintEnabled = enabled; }
        bool IsPaintEnabled() const { return m_bPaintEnabled; }
        
    protected:
        bool m_bPaintEnabled;
    };
    
    class CButton : public CBasePanel
    {
    public:
        CButton() : m_bPressed(false), m_bArmed(false) {}
        virtual ~CButton() {}
        
        void SetText(const wchar_t* text)
        {
            m_Text = text ? text : L"";
        }
        
        const wchar_t* GetText() const
        {
            return m_Text.c_str();
        }
        
        void Paint() override
        {
            ISurface* pSurface = (ISurface*)CreateSurfaceReal();
            if (!pSurface) return;
            
            int x, y, wide, tall;
            GetPos(x, y);
            GetSize(wide, tall);
            
            if (m_bPressed)
            {
                pSurface->DrawSetColor(150, 150, 150, 255);
            }
            else if (m_bArmed)
            {
                pSurface->DrawSetColor(220, 220, 220, 255);
            }
            else
            {
                pSurface->DrawSetColor(200, 200, 200, 255);
            }
            pSurface->DrawFilledRect(x, y, x + wide, y + tall);
            
            pSurface->DrawSetColor(80, 80, 80, 255);
            pSurface->DrawOutlinedRect(x, y, x + wide, y + tall);
            
            if (!m_Text.empty())
            {
                pSurface->DrawSetTextColor(0, 0, 0, 255);
                pSurface->DrawSetTextPos(x + 5, y + (tall - 12) / 2);
                pSurface->DrawPrintText(m_Text.c_str(), (int)m_Text.length());
            }
        }
        
        void SetPressed(bool pressed) { m_bPressed = pressed; }
        bool IsPressed() const { return m_bPressed; }
        void SetArmed(bool armed) { m_bArmed = armed; }
        bool IsArmed() const { return m_bArmed; }
        
    private:
        std::wstring m_Text;
        bool m_bPressed;
        bool m_bArmed;
    };
    
    class CLabel : public CBasePanel
    {
    public:
        CLabel() {}
        virtual ~CLabel() {}
        
        void SetText(const wchar_t* text)
        {
            m_Text = text ? text : L"";
        }
        
        const wchar_t* GetText() const
        {
            return m_Text.c_str();
        }
        
        void Paint() override
        {
            ISurface* pSurface = (ISurface*)CreateSurfaceReal();
            if (!pSurface) return;
            
            int x, y, wide, tall;
            GetPos(x, y);
            GetSize(wide, tall);
            
            pSurface->DrawSetColor(0, 0, 0, 0);
            pSurface->DrawFilledRect(x, y, x + wide, y + tall);
            
            if (!m_Text.empty())
            {
                pSurface->DrawSetTextColor(0, 0, 0, 255);
                pSurface->DrawSetTextPos(x, y + (tall - 12) / 2);
                pSurface->DrawPrintText(m_Text.c_str(), (int)m_Text.length());
            }
        }
        
    private:
        std::wstring m_Text;
    };
}
