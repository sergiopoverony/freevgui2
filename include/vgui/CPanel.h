#pragma once
#include "VPanel.h"
#include "IKeyValues.h"
#include <string>
#include <functional>

namespace vgui
{
    struct Message
    {
        std::string name;
        void* data;
        HPanel sender;
    };
    
    typedef std::function<void(Message&)> MessageHandler;
    
    class CPanel : public VPanel
    {
    public:
        CPanel(HPanel parentId = INVALID_PANEL, const char* panelName = nullptr);
        virtual ~CPanel();
        
        virtual void Paint();
        virtual void PaintBackground();
        virtual void PaintBuildOverlay();
        virtual void PerformLayout();
        virtual void ApplySchemeSettings(void* scheme);
        virtual void OnSizeChanged(int wide, int tall);
        virtual void OnMousePressed(int code);
        virtual void OnMouseReleased(int code);
        virtual void OnMouseWheeled(int delta);
        virtual void OnCursorMoved(int x, int y);
        virtual void OnCursorEntered();
        virtual void OnCursorExited();
        virtual void OnKeyCodePressed(int code);
        virtual void OnKeyCodeReleased(int code);
        virtual void OnKeyCodeTyped(int code);
        virtual void OnThink();
        virtual void OnCommand(const char* command);
        virtual void OnSetFocus();
        virtual void OnKillFocus();
        
        void PostMessage(HPanel target, const char* messageName, void* data = nullptr);
        void AddActionSignalTarget(CPanel* target);
        void RemoveActionSignalTarget(CPanel* target);
        
        void SetParent(CPanel* parent);
        CPanel* GetParent() const { return m_pParent; }
        
        void SetVisible(bool state);
        bool IsVisible() const { return m_bVisible; }
        void SetEnabled(bool state);
        bool IsEnabled() const { return m_bEnabled; }
        void SetBgColor(int r, int g, int b, int a);
        void SetFgColor(int r, int g, int b, int a);
        
        void InvalidateLayout(bool layoutNow = false, bool reloadScheme = false);
        void Repaint();
        
        void InternalCursorMoved(int x, int y);
        void InternalMousePressed(int code);
        void InternalMouseReleased(int code);
        void InternalMouseWheeled(int delta);
        void InternalKeyCodePressed(int code);
        void InternalKeyCodeReleased(int code);
        void InternalKeyCodeTyped(int code);
        
    protected:
        virtual void OnMessage(Message& msg, CPanel* sender);
        
        CPanel* m_pParent;
        bool m_bVisible;
        bool m_bEnabled;
        bool m_bNeedsLayout;
        bool m_bNeedsRepaint;
        unsigned char m_BgColor[4];
        unsigned char m_FgColor[4];
        std::vector<CPanel*> m_ActionTargets;
        std::vector<std::pair<std::string, MessageHandler>> m_MessageHandlers;
    };
}
