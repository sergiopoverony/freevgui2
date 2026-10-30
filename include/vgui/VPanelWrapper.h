#pragma once
#include "IPanel.h"
#include "VPanel.h"

namespace vgui
{
    class VPanelWrapper : public IPanel
    {
    public:
        VPanelWrapper();
        virtual ~VPanelWrapper();
        
        virtual void Init(HPanel vguiPanel);
        virtual void SetPos(HPanel vguiPanel, int x, int y);
        virtual void GetPos(HPanel vguiPanel, int& x, int& y);
        virtual void SetSize(HPanel vguiPanel, int wide, int tall);
        virtual void GetSize(HPanel vguiPanel, int& wide, int& tall);
        virtual void SetMinimumSize(HPanel vguiPanel, int wide, int tall);
        virtual void GetMinimumSize(HPanel vguiPanel, int& wide, int& tall);
        virtual void SetZPos(HPanel vguiPanel, int z);
        virtual int GetZPos(HPanel vguiPanel);
        virtual void GetClipRect(HPanel vguiPanel, int& x0, int& y0, int& x1, int& y1);
        virtual void SetClipRect(HPanel vguiPanel, int x0, int y0, int x1, int y1);
        virtual void Solve(HPanel vguiPanel);
        virtual const char* GetName(HPanel vguiPanel);
        virtual const char* GetClassName(HPanel vguiPanel);
        virtual void SetName(HPanel vguiPanel, const char* name);
        virtual bool IsVisible(HPanel vguiPanel);
        virtual void SetVisible(HPanel vguiPanel, bool state);
        virtual bool IsEnabled(HPanel vguiPanel);
        virtual void SetEnabled(HPanel vguiPanel, bool state);
        virtual HPanel GetParent(HPanel vguiPanel);
        virtual int GetChildCount(HPanel vguiPanel);
        virtual HPanel GetChild(HPanel vguiPanel, int index);
        virtual bool HasFocus(HPanel vguiPanel);
        virtual void RequestFocus(HPanel vguiPanel, int direction);
        virtual bool IsPopup(HPanel vguiPanel);
        virtual void SetPopup(HPanel vguiPanel, bool state);
        virtual HPanel GetTopmostPopup();
        virtual void SetTopmostPopup(HPanel vguiPanel, bool state);
        virtual void* GetClientPanel(HPanel vguiPanel);
        
    private:
        VPanel* GetVPanel(HPanel vguiPanel);
    };
}
