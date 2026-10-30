#include "vgui/CPanel.h"
#include "vgui/ISurface.h"
#include <algorithm>

namespace vgui
{
    extern void* CreateSurfaceReal();
    extern HPanel AllocatePanel(const char* className, HPanel parentId);
    extern VPanel* GetVPanel(HPanel panel);
    
    CPanel::CPanel(HPanel parentId, const char* panelName)
        : m_pParent(nullptr)
        , m_bVisible(true)
        , m_bEnabled(true)
        , m_bNeedsLayout(true)
        , m_bNeedsRepaint(true)
    {
        m_BgColor[0] = m_BgColor[1] = m_BgColor[2] = 200;
        m_BgColor[3] = 255;
        m_FgColor[0] = m_FgColor[1] = m_FgColor[2] = 0;
        m_FgColor[3] = 255;
        
        if (panelName) SetName(panelName);
        SetClassName("Panel");
    }
    
    CPanel::~CPanel()
    {
    }
    
    void CPanel::Paint()
    {
        PaintBackground();
    }
    
    void CPanel::PaintBackground()
    {
        if (!m_bVisible) return;
        
        ISurface* pSurface = (ISurface*)CreateSurfaceReal();
        if (!pSurface) return;
        
        int x, y, wide, tall;
        GetPos(x, y);
        GetSize(wide, tall);
        
        pSurface->DrawSetColor(m_BgColor[0], m_BgColor[1], m_BgColor[2], m_BgColor[3]);
        pSurface->DrawFilledRect(x, y, x + wide, y + tall);
    }
    
    void CPanel::PaintBuildOverlay()
    {
    }
    
    void CPanel::PerformLayout()
    {
        m_bNeedsLayout = false;
    }
    
    void CPanel::ApplySchemeSettings(void* scheme)
    {
    }
    
    void CPanel::OnSizeChanged(int wide, int tall)
    {
    }
    
    void CPanel::OnMousePressed(int code)
    {
    }
    
    void CPanel::OnMouseReleased(int code)
    {
    }
    
    void CPanel::OnMouseWheeled(int delta)
    {
    }
    
    void CPanel::OnCursorMoved(int x, int y)
    {
    }
    
    void CPanel::OnCursorEntered()
    {
    }
    
    void CPanel::OnCursorExited()
    {
    }
    
    void CPanel::OnKeyCodePressed(int code)
    {
    }
    
    void CPanel::OnKeyCodeReleased(int code)
    {
    }
    
    void CPanel::OnKeyCodeTyped(int code)
    {
    }
    
    void CPanel::OnThink()
    {
    }
    
    void CPanel::OnCommand(const char* command)
    {
    }
    
    void CPanel::OnSetFocus()
    {
    }
    
    void CPanel::OnKillFocus()
    {
    }
    
    void CPanel::PostMessage(HPanel target, const char* messageName, void* data)
    {
        VPanel* pTarget = GetVPanel(target);
        if (!pTarget || !messageName) return;
        
        CPanel* pPanel = dynamic_cast<CPanel*>(pTarget);
        if (!pPanel) return;
        
        Message msg;
        msg.name = messageName;
        msg.data = data;
        msg.sender = GetID();
        
        pPanel->OnMessage(msg, this);
    }
    
    void CPanel::AddActionSignalTarget(CPanel* target)
    {
        if (!target) return;
        auto it = std::find(m_ActionTargets.begin(), m_ActionTargets.end(), target);
        if (it == m_ActionTargets.end())
        {
            m_ActionTargets.push_back(target);
        }
    }
    
    void CPanel::RemoveActionSignalTarget(CPanel* target)
    {
        auto it = std::find(m_ActionTargets.begin(), m_ActionTargets.end(), target);
        if (it != m_ActionTargets.end())
        {
            m_ActionTargets.erase(it);
        }
    }
    
    void CPanel::SetParent(CPanel* parent)
    {
        m_pParent = parent;
        VPanel::SetParent(parent);
    }
    
    void CPanel::SetVisible(bool state)
    {
        m_bVisible = state;
        VPanel::SetVisible(state);
    }
    
    void CPanel::SetEnabled(bool state)
    {
        m_bEnabled = state;
        VPanel::SetEnabled(state);
    }
    
    void CPanel::SetBgColor(int r, int g, int b, int a)
    {
        m_BgColor[0] = (unsigned char)r;
        m_BgColor[1] = (unsigned char)g;
        m_BgColor[2] = (unsigned char)b;
        m_BgColor[3] = (unsigned char)a;
    }
    
    void CPanel::SetFgColor(int r, int g, int b, int a)
    {
        m_FgColor[0] = (unsigned char)r;
        m_FgColor[1] = (unsigned char)g;
        m_FgColor[2] = (unsigned char)b;
        m_FgColor[3] = (unsigned char)a;
    }
    
    void CPanel::InvalidateLayout(bool layoutNow, bool reloadScheme)
    {
        m_bNeedsLayout = true;
        if (layoutNow)
        {
            PerformLayout();
        }
    }
    
    void CPanel::Repaint()
    {
        m_bNeedsRepaint = true;
    }
    
    void CPanel::InternalCursorMoved(int x, int y)
    {
        if (!m_bEnabled) return;
        OnCursorMoved(x, y);
    }
    
    void CPanel::InternalMousePressed(int code)
    {
        if (!m_bEnabled) return;
        OnMousePressed(code);
    }
    
    void CPanel::InternalMouseReleased(int code)
    {
        if (!m_bEnabled) return;
        OnMouseReleased(code);
    }
    
    void CPanel::InternalMouseWheeled(int delta)
    {
        if (!m_bEnabled) return;
        OnMouseWheeled(delta);
    }
    
    void CPanel::InternalKeyCodePressed(int code)
    {
        if (!m_bEnabled) return;
        OnKeyCodePressed(code);
    }
    
    void CPanel::InternalKeyCodeReleased(int code)
    {
        if (!m_bEnabled) return;
        OnKeyCodeReleased(code);
    }
    
    void CPanel::InternalKeyCodeTyped(int code)
    {
        if (!m_bEnabled) return;
        OnKeyCodeTyped(code);
    }
    
    void CPanel::OnMessage(Message& msg, CPanel* sender)
    {
        for (auto& handler : m_MessageHandlers)
        {
            if (handler.first == msg.name)
            {
                handler.second(msg);
            }
        }
    }
}
