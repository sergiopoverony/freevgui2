#include "vgui/VPanelWrapper.h"

namespace vgui
{
    extern VPanel* GetVPanel(HPanel panel);

    VPanelWrapper::VPanelWrapper()
    {
    }

    VPanelWrapper::~VPanelWrapper()
    {
    }

    VPanel* VPanelWrapper::GetVPanel(HPanel vguiPanel)
    {
        return vgui::GetVPanel(vguiPanel);
    }

    void VPanelWrapper::Init(HPanel vguiPanel)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->Init();
    }

    void VPanelWrapper::SetPos(HPanel vguiPanel, int x, int y)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->SetPos(x, y);
    }

    void VPanelWrapper::GetPos(HPanel vguiPanel, int& x, int& y)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->GetPos(x, y);
        else { x = y = 0; }
    }

    void VPanelWrapper::SetSize(HPanel vguiPanel, int wide, int tall)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->SetSize(wide, tall);
    }

    void VPanelWrapper::GetSize(HPanel vguiPanel, int& wide, int& tall)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->GetSize(wide, tall);
        else { wide = tall = 0; }
    }

    void VPanelWrapper::SetMinimumSize(HPanel vguiPanel, int wide, int tall) {}
    void VPanelWrapper::GetMinimumSize(HPanel vguiPanel, int& wide, int& tall) { wide = tall = 0; }
    void VPanelWrapper::SetZPos(HPanel vguiPanel, int z) {}
    int VPanelWrapper::GetZPos(HPanel vguiPanel) { return 0; }
    void VPanelWrapper::GetClipRect(HPanel vguiPanel, int& x0, int& y0, int& x1, int& y1) { x0 = y0 = x1 = y1 = 0; }
    void VPanelWrapper::SetClipRect(HPanel vguiPanel, int x0, int y0, int x1, int y1) {}
    void VPanelWrapper::Solve(HPanel vguiPanel) {}

    const char* VPanelWrapper::GetName(HPanel vguiPanel)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        return pPanel ? pPanel->GetName() : "";
    }

    const char* VPanelWrapper::GetClassName(HPanel vguiPanel)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        return pPanel ? pPanel->GetClassName() : "";
    }

    void VPanelWrapper::SetName(HPanel vguiPanel, const char* name)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->SetName(name);
    }

    bool VPanelWrapper::IsVisible(HPanel vguiPanel)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        return pPanel ? pPanel->IsVisible() : false;
    }

    void VPanelWrapper::SetVisible(HPanel vguiPanel, bool state)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->SetVisible(state);
    }

    bool VPanelWrapper::IsEnabled(HPanel vguiPanel)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        return pPanel ? pPanel->IsEnabled() : false;
    }

    void VPanelWrapper::SetEnabled(HPanel vguiPanel, bool state)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel) pPanel->SetEnabled(state);
    }

    HPanel VPanelWrapper::GetParent(HPanel vguiPanel)
    {
        VPanel* pPanel = GetVPanel(vguiPanel);
        if (pPanel)
        {
            VPanel* pParent = pPanel->GetParent();
            if (pParent) return pParent->GetID();
        }
        return INVALID_PANEL;
    }

    int VPanelWrapper::GetChildCount(HPanel vguiPanel) { return 0; }
    HPanel VPanelWrapper::GetChild(HPanel vguiPanel, int index) { return INVALID_PANEL; }
    bool VPanelWrapper::HasFocus(HPanel vguiPanel) { return false; }
    void VPanelWrapper::RequestFocus(HPanel vguiPanel, int direction) {}
    bool VPanelWrapper::IsPopup(HPanel vguiPanel) { return false; }
    void VPanelWrapper::SetPopup(HPanel vguiPanel, bool state) {}
    HPanel VPanelWrapper::GetTopmostPopup() { return INVALID_PANEL; }
    void VPanelWrapper::SetTopmostPopup(HPanel vguiPanel, bool state) {}
    void* VPanelWrapper::GetClientPanel(HPanel vguiPanel) { return nullptr; }
}
