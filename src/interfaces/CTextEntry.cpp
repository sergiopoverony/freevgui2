#include "vgui/CTextEntry.h"
#include "vgui/ISurface.h"
#include <windows.h>

namespace vgui
{
    extern void* CreateSurfaceReal();
    
    CTextEntry::CTextEntry(HPanel parentId, const char* panelName)
        : CPanel(parentId, panelName)
        , m_nCursorPos(0)
        , m_nMaxLength(256)
        , m_bNumericOnly(false)
        , m_bEditable(true)
        , m_bHasFocus(false)
        , m_nBlinkCounter(0)
    {
        SetClassName("TextEntry");
        SetSize(150, 20);
        SetBgColor(255, 255, 255, 255);
    }
    
    CTextEntry::~CTextEntry()
    {
    }
    
    void CTextEntry::SetText(const wchar_t* text)
    {
        m_Text = text ? text : L"";
        if (m_nCursorPos > (int)m_Text.length())
        {
            m_nCursorPos = (int)m_Text.length();
        }
        Repaint();
    }
    
    void CTextEntry::SetCursorPos(int pos)
    {
        if (pos < 0) pos = 0;
        if (pos > (int)m_Text.length()) pos = (int)m_Text.length();
        m_nCursorPos = pos;
        Repaint();
    }
    
    void CTextEntry::Paint()
    {
        PaintBackground();
    }
    
    void CTextEntry::PaintBackground()
    {
        if (!IsVisible()) return;
        
        ISurface* pSurface = (ISurface*)CreateSurfaceReal();
        if (!pSurface) return;
        
        int x, y, wide, tall;
        GetPos(x, y);
        GetSize(wide, tall);
        
        // Background
        pSurface->DrawSetColor(m_BgColor[0], m_BgColor[1], m_BgColor[2], m_BgColor[3]);
        pSurface->DrawFilledRect(x, y, x + wide, y + tall);
        
        // Border
        pSurface->DrawSetColor(100, 100, 100, 255);
        pSurface->DrawOutlinedRect(x, y, x + wide, y + tall);
        
        // Text
        if (!m_Text.empty())
        {
            pSurface->DrawSetTextColor(0, 0, 0, 255);
            pSurface->DrawSetTextPos(x + 4, y + (tall - 12) / 2);
            pSurface->DrawPrintText(m_Text.c_str(), (int)m_Text.length());
        }
        
        // Cursor
        if (m_bHasFocus && (m_nBlinkCounter / 30) % 2 == 0)
        {
            int cursorX = x + 4;
            if (!m_Text.empty() && m_nCursorPos > 0)
            {
                int textWide, textTall;
                pSurface->GetTextSize(0, m_Text.c_str(), textWide, textTall);
                cursorX = x + 4 + (textWide * m_nCursorPos) / (int)m_Text.length();
            }
            pSurface->DrawSetColor(0, 0, 0, 255);
            pSurface->DrawLine(cursorX, y + 2, cursorX, y + tall - 2);
        }
        
        m_nBlinkCounter++;
    }
    
    void CTextEntry::OnMousePressed(int code)
    {
        if (code == 0)
        {
            m_bHasFocus = true;
            Repaint();
        }
        CPanel::OnMousePressed(code);
    }
    
    void CTextEntry::OnKeyCodeTyped(int code)
    {
        if (!m_bEditable)
        {
            CPanel::OnKeyCodeTyped(code);
            return;
        }
        
        if (code == VK_BACK)
        {
            if (m_nCursorPos > 0 && !m_Text.empty())
            {
                m_Text.erase(m_nCursorPos - 1, 1);
                m_nCursorPos--;
                Repaint();
            }
        }
        else if (code == VK_DELETE)
        {
            if (m_nCursorPos < (int)m_Text.length())
            {
                m_Text.erase(m_nCursorPos, 1);
                Repaint();
            }
        }
        else if (code == VK_LEFT)
        {
            if (m_nCursorPos > 0) m_nCursorPos--;
            Repaint();
        }
        else if (code == VK_RIGHT)
        {
            if (m_nCursorPos < (int)m_Text.length()) m_nCursorPos++;
            Repaint();
        }
        else if (code == VK_HOME)
        {
            m_nCursorPos = 0;
            Repaint();
        }
        else if (code == VK_END)
        {
            m_nCursorPos = (int)m_Text.length();
            Repaint();
        }
        else if (code >= 32 && code < 127)
        {
            wchar_t ch = (wchar_t)code;
            if (m_bNumericOnly && (ch < L'0' || ch > L'9'))
            {
                CPanel::OnKeyCodeTyped(code);
                return;
            }
            
            if ((int)m_Text.length() < m_nMaxLength)
            {
                m_Text.insert(m_nCursorPos, 1, ch);
                m_nCursorPos++;
                Repaint();
            }
        }
        
        CPanel::OnKeyCodeTyped(code);
    }
    
    void CTextEntry::OnKeyCodeReleased(int code)
    {
        CPanel::OnKeyCodeReleased(code);
    }
    
    void CTextEntry::OnCursorMoved(int x, int y)
    {
        CPanel::OnCursorMoved(x, y);
    }
}
