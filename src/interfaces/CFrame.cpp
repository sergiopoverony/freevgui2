#include "vgui/CFrame.h"
#include "vgui/ISurface.h"
#include <windows.h>

namespace vgui
{
    extern void* CreateSurfaceReal();
    
    CFrame::CFrame(HPanel parentId, const char* panelName)
        : CPanel(parentId, panelName)
        , m_bSizeable(true)
        , m_bMoveable(true)
        , m_bCloseButtonVisible(true)
        , m_bMinimizeButtonVisible(true)
        , m_bDragging(false)
        , m_nDragStartX(0)
        , m_nDragStartY(0)
        , m_nPanelStartX(0)
        , m_nPanelStartY(0)
    {
        SetClassName("Frame");
        SetSize(400, 300);
        SetBgColor(240, 240, 240, 255);
    }
    
    CFrame::~CFrame()
    {
    }
    
    void CFrame::SetTitle(const wchar_t* title)
    {
        m_Title = title ? title : L"";
    }
    
    void CFrame::Activate()
    {
        SetVisible(true);
        Repaint();
    }
    
    void CFrame::Close()
    {
        SetVisible(false);
    }
    
    void CFrame::Paint()
    {
        PaintBackground();
    }
    
    void CFrame::PaintBackground()
    {
        if (!IsVisible()) return;
        
        ISurface* pSurface = (ISurface*)CreateSurfaceReal();
        if (!pSurface) return;
        
        int x, y, wide, tall;
        GetPos(x, y);
        GetSize(wide, tall);
        
        pSurface->DrawSetColor(60, 120, 200, 255);
        pSurface->DrawFilledRect(x, y, x + wide, y + 24);
        
        if (!m_Title.empty())
        {
            pSurface->DrawSetTextColor(255, 255, 255, 255);
            pSurface->DrawSetTextPos(x + 8, y + 4);
            pSurface->DrawPrintText(m_Title.c_str(), (int)m_Title.length());
        }
        
        if (m_bCloseButtonVisible)
        {
            pSurface->DrawSetColor(200, 60, 60, 255);
            pSurface->DrawFilledRect(x + wide - 24, y, x + wide, y + 24);
            pSurface->DrawSetTextColor(255, 255, 255, 255);
            pSurface->DrawSetTextPos(x + wide - 16, y + 4);
            pSurface->DrawPrintText(L"X", 1);
        }
        
        pSurface->DrawSetColor(m_BgColor[0], m_BgColor[1], m_BgColor[2], m_BgColor[3]);
        pSurface->DrawFilledRect(x, y + 24, x + wide, y + tall);
        
        pSurface->DrawSetColor(80, 80, 80, 255);
        pSurface->DrawOutlinedRect(x, y, x + wide, y + tall);
    }
    
    void CFrame::PerformLayout()
    {
        CPanel::PerformLayout();
    }
    
    void CFrame::OnMousePressed(int code)
    {
        if (code == 0 && m_bMoveable)
        {
            int x, y;
            GetPos(x, y);
            
            POINT pt;
            ::GetCursorPos(&pt);
            
            if (pt.y >= y && pt.y < y + 24)
            {
                m_bDragging = true;
                m_nDragStartX = pt.x;
                m_nDragStartY = pt.y;
                m_nPanelStartX = x;
                m_nPanelStartY = y;
            }
        }
        
        CPanel::OnMousePressed(code);
    }
    
    void CFrame::OnMouseReleased(int code)
    {
        if (code == 0)
        {
            m_bDragging = false;
        }
        CPanel::OnMouseReleased(code);
    }
    
    void CFrame::OnCursorMoved(int x, int y)
    {
        if (m_bDragging)
        {
            int dx = x - m_nDragStartX;
            int dy = y - m_nDragStartY;
            SetPos(m_nPanelStartX + dx, m_nPanelStartY + dy);
            Repaint();
        }
        CPanel::OnCursorMoved(x, y);
    }
    
    void CFrame::OnKeyCodeTyped(int code)
    {
        if (code == VK_ESCAPE)
        {
            Close();
        }
        CPanel::OnKeyCodeTyped(code);
    }
}
