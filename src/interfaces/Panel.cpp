#include "vgui/IPanel.h"
#include <cstring>

namespace vgui
{
    extern void Log(const char* format, ...);
    
    class CPanelWrapper : public IPanel
    {
    public:
        CPanelWrapper() { Log("CPanelWrapper::CPanelWrapper()\n"); }
        ~CPanelWrapper() {}
        
        void Init(HPanel vguiPanel) override { Log("IPanel::Init(%u)\n", vguiPanel); }
        void SetPos(HPanel vguiPanel, int x, int y) override { Log("IPanel::SetPos(%u, %d, %d)\n", vguiPanel, x, y); }
        void GetPos(HPanel vguiPanel, int& x, int& y) override { Log("IPanel::GetPos(%u)\n", vguiPanel); x = 0; y = 0; }
        void SetSize(HPanel vguiPanel, int wide, int tall) override { Log("IPanel::SetSize(%u, %d, %d)\n", vguiPanel, wide, tall); }
        void GetSize(HPanel vguiPanel, int& wide, int& tall) override { Log("IPanel::GetSize(%u)\n", vguiPanel); wide = 100; tall = 100; }
        void SetMinimumSize(HPanel vguiPanel, int wide, int tall) override { Log("IPanel::SetMinimumSize(%u, %d, %d)\n", vguiPanel, wide, tall); }
        void GetMinimumSize(HPanel vguiPanel, int& wide, int& tall) override { Log("IPanel::GetMinimumSize(%u)\n", vguiPanel); wide = 0; tall = 0; }
        void SetZPos(HPanel vguiPanel, int z) override { Log("IPanel::SetZPos(%u, %d)\n", vguiPanel, z); }
        int GetZPos(HPanel vguiPanel) override { Log("IPanel::GetZPos(%u)\n", vguiPanel); return 0; }
        void GetClipRect(HPanel vguiPanel, int& x0, int& y0, int& x1, int& y1) override { Log("IPanel::GetClipRect(%u)\n", vguiPanel); x0 = 0; y0 = 0; x1 = 100; y1 = 100; }
        void SetClipRect(HPanel vguiPanel, int x0, int y0, int x1, int y1) override { Log("IPanel::SetClipRect(%u)\n", vguiPanel); }
        void Solve(HPanel vguiPanel) override { Log("IPanel::Solve(%u)\n", vguiPanel); }
        const char* GetName(HPanel vguiPanel) override { Log("IPanel::GetName(%u)\n", vguiPanel); return "Panel"; }
        const char* GetClassName(HPanel vguiPanel) override { Log("IPanel::GetClassName(%u)\n", vguiPanel); return "Panel"; }
        void SetName(HPanel vguiPanel, const char* name) override { Log("IPanel::SetName(%u, '%s')\n", vguiPanel, name ? name : "null"); }
        bool IsVisible(HPanel vguiPanel) override { Log("IPanel::IsVisible(%u)\n", vguiPanel); return true; }
        void SetVisible(HPanel vguiPanel, bool state) override { Log("IPanel::SetVisible(%u, %d)\n", vguiPanel, state); }
        bool IsEnabled(HPanel vguiPanel) override { Log("IPanel::IsEnabled(%u)\n", vguiPanel); return true; }
        void SetEnabled(HPanel vguiPanel, bool state) override { Log("IPanel::SetEnabled(%u, %d)\n", vguiPanel, state); }
        HPanel GetParent(HPanel vguiPanel) override { Log("IPanel::GetParent(%u)\n", vguiPanel); return INVALID_PANEL; }
        int GetChildCount(HPanel vguiPanel) override { Log("IPanel::GetChildCount(%u)\n", vguiPanel); return 0; }
        HPanel GetChild(HPanel vguiPanel, int index) override { Log("IPanel::GetChild(%u, %d)\n", vguiPanel, index); return INVALID_PANEL; }
        bool HasFocus(HPanel vguiPanel) override { Log("IPanel::HasFocus(%u)\n", vguiPanel); return false; }
        void RequestFocus(HPanel vguiPanel, int direction) override { Log("IPanel::RequestFocus(%u)\n", vguiPanel); }
        bool IsPopup(HPanel vguiPanel) override { Log("IPanel::IsPopup(%u)\n", vguiPanel); return false; }
        void SetPopup(HPanel vguiPanel, bool state) override { Log("IPanel::SetPopup(%u, %d)\n", vguiPanel, state); }
        HPanel GetTopmostPopup() override { Log("IPanel::GetTopmostPopup()\n"); return INVALID_PANEL; }
        void SetTopmostPopup(HPanel vguiPanel, bool state) override { Log("IPanel::SetTopmostPopup(%u, %d)\n", vguiPanel, state); }
        void* GetClientPanel(HPanel vguiPanel) override { Log("IPanel::GetClientPanel(%u)\n", vguiPanel); return nullptr; }
    };
    
    static CPanelWrapper g_PanelWrapperInstance;
    
    void* CreatePanelReal()
    {
        Log("CreatePanelReal() returning %p\n", &g_PanelWrapperInstance);
        return &g_PanelWrapperInstance;
    }
}
