#pragma once
#include "VGui.h"

namespace vgui
{
    class IPanel
    {
    public:
        virtual void Init(HPanel vguiPanel) = 0;
        virtual void SetPos(HPanel vguiPanel, int x, int y) = 0;
        virtual void GetPos(HPanel vguiPanel, int& x, int& y) = 0;
        virtual void SetSize(HPanel vguiPanel, int wide, int tall) = 0;
        virtual void GetSize(HPanel vguiPanel, int& wide, int& tall) = 0;
        virtual void SetMinimumSize(HPanel vguiPanel, int wide, int tall) = 0;
        virtual void GetMinimumSize(HPanel vguiPanel, int& wide, int& tall) = 0;
        virtual void SetZPos(HPanel vguiPanel, int z) = 0;
        virtual int GetZPos(HPanel vguiPanel) = 0;
        virtual void GetClipRect(HPanel vguiPanel, int& x0, int& y0, int& x1, int& y1) = 0;
        virtual void SetClipRect(HPanel vguiPanel, int x0, int y0, int x1, int y1) = 0;
        virtual void Solve(HPanel vguiPanel) = 0;
        virtual const char* GetName(HPanel vguiPanel) = 0;
        virtual const char* GetClassName(HPanel vguiPanel) = 0;
        virtual void SetName(HPanel vguiPanel, const char* name) = 0;
        virtual bool IsVisible(HPanel vguiPanel) = 0;
        virtual void SetVisible(HPanel vguiPanel, bool state) = 0;
        virtual bool IsEnabled(HPanel vguiPanel) = 0;
        virtual void SetEnabled(HPanel vguiPanel, bool state) = 0;
        virtual HPanel GetParent(HPanel vguiPanel) = 0;
        virtual int GetChildCount(HPanel vguiPanel) = 0;
        virtual HPanel GetChild(HPanel vguiPanel, int index) = 0;
        virtual bool HasFocus(HPanel vguiPanel) = 0;
        virtual void RequestFocus(HPanel vguiPanel, int direction = 0) = 0;
        virtual bool IsPopup(HPanel vguiPanel) = 0;
        virtual void SetPopup(HPanel vguiPanel, bool state) = 0;
        virtual HPanel GetTopmostPopup() = 0;
        virtual void SetTopmostPopup(HPanel vguiPanel, bool state) = 0;
        virtual void* GetClientPanel(HPanel vguiPanel) = 0;
    };
    
    #define VGUI_PANEL_INTERFACE_VERSION "VGUI_Panel007"
}