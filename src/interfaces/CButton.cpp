#include "vgui/CButton.h"
#include "vgui/ISurface.h"
#include <windows.h>

namespace vgui
{
    extern void* CreateSurfaceReal();
    
    CButton::CButton(HPanel parentId, const char* panelName)
        : CPanel(parentId, panelName)
        , m_bArmed(false)
        , m_bDepressed(false)
    {
        SetClassName("Button");
        SetSize(80, 24);
        SetBgColor(220, 220, 220, 255);
    }
    
    CButton::~CButton()
    {
    }
    
    void CButton::SetText(const wchar_t* text)
    {
        m_Text = text ? text : L"";
        Repaint();
    }
    
    void CButton::SetCommand(const char* command)
    {
        m_Command = command ? command : "";
    }
    
    void CButton::Paint()
    {
        PaintBackground();
    }
    
    void CButton::PaintBackground()
    {
        if (!IsVisible()) return;
        
        ISurface* pSurface = (ISurface*)CreateSurfaceReal();
        if (!pSurface) return;
        
        int x, y, wide, tall;
        GetPos(x, y);
        GetSize(wide, tall);
        
        if (m_bDepressed)
        {
            pSurface->DrawSetColor(180, 180, 180, 255);
        }
        else if (m_bArmed)
        {
            pSurface->DrawSetColor(240, 240, 240, 255);
        }
        else
        {
            pSurface->DrawSetColor(m_BgColor[0], m_BgColor[1], m_BgColor[2], m_BgColor[3]);
        }
        
        pSurface->DrawFilledRect(x, y, x + wide, y + tall);
        
        pSurface->DrawSetColor(100, 100, 100, 255);
        pSurface->DrawOutlinedRect(x, y, x + wide, y + tall);
        
        if (!m_Text.empty())
        {
            pSurface->DrawSetTextColor(0, 0, 0, 255);
            
            int textWide, textTall;
            pSurface->GetTextSize(0, m_Text.c_str(), textWide, textTall);
            int textX = x + (wide - textWide) / 2;
            int textY = y + (tall - textTall) / 2;
            
            pSurface->DrawSetTextPos(textX, textY);
            pSurface->DrawPrintText(m_Text.c_str(), (int)m_Text.length());
        }
    }
    
    void CButton::OnMousePressed(int code)
    {
        if (code == 0 && IsEnabled())
        {
            m_bDepressed = true;
            Repaint();
        }
        CPanel::OnMousePressed(code);
    }
    
    void CButton::OnMouseReleased(int code)
    {
        if (code == 0 && m_bDepressed)
        {
            m_bDepressed = false;
            
            if (m_bArmed && !m_Command.empty())
            {
                for (auto* pTarget : m_ActionTargets)
                {
                    if (pTarget)
                    {
                        pTarget->OnCommand(m_Command.c_str());
                    }
                }
            }
            
            Repaint();
        }
        CPanel::OnMouseReleased(code);
    }
    
    void CButton::OnCursorMoved(int x, int y)
    {
        int wide, tall;
        GetSize(wide, tall);
        
        bool bInside = (x >= 0 && x < wide && y >= 0 && y < tall);
        
        if (bInside != m_bArmed)
        {
            m_bArmed = bInside;
            Repaint();
        }
        
        CPanel::OnCursorMoved(x, y);
    }
    
    void CButton::OnCursorEntered()
    {
        m_bArmed = true;
        Repaint();
        CPanel::OnCursorEntered();
    }
    
    void CButton::OnCursorExited()
    {
        m_bArmed = false;
        m_bDepressed = false;
        Repaint();
        CPanel::OnCursorExited();
    }
    
    void CButton::OnKeyCodeTyped(int code)
    {
        if (code == VK_RETURN || code == VK_SPACE)
        {
            if (!m_Command.empty())
            {
                for (auto* pTarget : m_ActionTargets)
                {
                    if (pTarget)
                    {
                        pTarget->OnCommand(m_Command.c_str());
                    }
                }
            }
        }
        CPanel::OnKeyCodeTyped(code);
    }
}
