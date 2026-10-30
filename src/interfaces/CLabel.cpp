#include "vgui/CLabel.h"
#include "vgui/ISurface.h"

namespace vgui
{
    extern void* CreateSurfaceReal();
    
    CLabel::CLabel(HPanel parentId, const char* panelName)
        : CPanel(parentId, panelName)
        , m_nContentAlignment(0)
    {
        SetClassName("Label");
        SetSize(100, 20);
        SetBgColor(0, 0, 0, 0);
    }
    
    CLabel::~CLabel()
    {
    }
    
    void CLabel::SetText(const wchar_t* text)
    {
        m_Text = text ? text : L"";
        Repaint();
    }
    
    void CLabel::SetContentAlignment(int align)
    {
        m_nContentAlignment = align;
        Repaint();
    }
    
    void CLabel::Paint()
    {
        PaintBackground();
    }
    
    void CLabel::PaintBackground()
    {
        if (!IsVisible()) return;
        
        ISurface* pSurface = (ISurface*)CreateSurfaceReal();
        if (!pSurface) return;
        
        int x, y, wide, tall;
        GetPos(x, y);
        GetSize(wide, tall);
        
        if (m_Text.empty()) return;
        
        pSurface->DrawSetTextColor(m_FgColor[0], m_FgColor[1], m_FgColor[2], m_FgColor[3]);
        
        int textWide, textTall;
        pSurface->GetTextSize(0, m_Text.c_str(), textWide, textTall);
        
        int textX = x;
        int textY = y + (tall - textTall) / 2;
        
        switch (m_nContentAlignment)
        {
        case 1: textX = x; break;
        case 2: textX = x + (wide - textWide) / 2; break;
        case 3: textX = x + wide - textWide; break;
        default: textX = x; break;
        }
        
        pSurface->DrawSetTextPos(textX, textY);
        pSurface->DrawPrintText(m_Text.c_str(), (int)m_Text.length());
    }
}
