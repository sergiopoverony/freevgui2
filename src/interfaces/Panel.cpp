#include "vgui/IPanel.h"
#include "vgui/VPanel.h"
#include <cstring>
#include <windows.h>

namespace vgui
{
    extern void Log(const char* format, ...);
    VGUI_API VPanel* GetVPanel(HPanel panel);   // dllmain.cpp

    // Logs only the first 24 calls from each call site, so the log stays readable
    // but still shows which IPanel methods the engine really uses (and in which order).
    // Name (without extension) of the DLL that contains `addr`; empty if unknown.
    // Used to work out which module a client panel belongs to: its vtable lives in that DLL.
    static void ModuleBaseNameFromAddress(const void* addr, char* out, size_t outSize)
    {
        out[0] = 0;
        HMODULE hMod = nullptr;
        if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)addr, &hMod) || !hMod)
            return;
        char path[MAX_PATH] = {0};
        if (!GetModuleFileNameA(hMod, path, MAX_PATH)) return;
        const char* name = strrchr(path, '\\');
        name = name ? name + 1 : path;
        size_t n = strlen(name);
        if (n > 4 && _stricmp(name + n - 4, ".dll") == 0) n -= 4;
        if (n >= outSize) n = outSize - 1;
        memcpy(out, name, n);
        out[n] = 0;
    }

    // Reads the vtable pointer of the client panel under SEH (the pointer comes from another module).
    static const void* ReadVtable(void* client)
    {
        __try { return client ? *(const void* const*)client : nullptr; }
        __except (EXCEPTION_EXECUTE_HANDLER) { return nullptr; }
    }

    #define TRACE(...) do { static int s_n = 0; if (s_n++ < 24) Log(__VA_ARGS__); } while (0)

    #define VGUI_TRACE_MODULE(vp, mod) TRACE("IPanel::Init(%lu) owner module = '%s'\n", (unsigned long)(vp), (mod))

    class CPanelImpl : public IPanel
    {
    public:
        CPanelImpl() { Log("CPanelImpl::CPanelImpl()\n"); }

        void Init(VPANEL vp, void* clientPanel) override
        {
            TRACE("IPanel::Init(%lu, client=%p)\n", vp, clientPanel);
            if (VPanel* p = P(vp))
            {
                p->m_pClient = clientPanel;
                ModuleBaseNameFromAddress(ReadVtable(clientPanel), p->m_szModule, sizeof(p->m_szModule));
                VGUI_TRACE_MODULE(vp, p->m_szModule);
            }
        }

        void SetPos(VPANEL vp, int x, int y) override
        {
            TRACE("IPanel::SetPos(%lu, %d, %d)\n", vp, x, y);
            if (VPanel* p = P(vp)) p->SetPos(x, y);
        }
        void GetPos(VPANEL vp, int& x, int& y) override
        {
            x = y = 0;
            if (VPanel* p = P(vp)) p->GetPos(x, y);
        }
        void SetSize(VPANEL vp, int wide, int tall) override
        {
            TRACE("IPanel::SetSize(%lu, %d, %d)\n", vp, wide, tall);
            if (VPanel* p = P(vp)) p->SetSize(wide, tall);
        }
        void GetSize(VPANEL vp, int& wide, int& tall) override
        {
            wide = tall = 0;
            if (VPanel* p = P(vp)) p->GetSize(wide, tall);
        }
        void SetMinimumSize(VPANEL vp, int wide, int tall) override
        {
            if (VPanel* p = P(vp)) { p->m_iMinWide = wide; p->m_iMinTall = tall; }
        }
        void GetMinimumSize(VPANEL vp, int& wide, int& tall) override
        {
            wide = tall = 0;
            if (VPanel* p = P(vp)) { wide = p->m_iMinWide; tall = p->m_iMinTall; }
        }
        void SetZPos(VPANEL vp, int z) override { if (VPanel* p = P(vp)) p->m_iZPos = z; }
        int GetZPos(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_iZPos : 0; }

        void GetAbsPos(VPANEL vp, int& x, int& y) override
        {
            x = y = 0;
            for (VPanel* p = P(vp); p; p = p->GetParent())
            {
                int px, py;
                p->GetPos(px, py);
                x += px; y += py;
            }
        }
        void GetClipRect(VPANEL vp, int& x0, int& y0, int& x1, int& y1) override
        {
            int w = 0, t = 0;
            GetAbsPos(vp, x0, y0);
            GetSize(vp, w, t);
            x1 = x0 + w; y1 = y0 + t;
        }
        void SetInset(VPANEL vp, int l, int t, int r, int b) override
        {
            if (VPanel* p = P(vp)) { p->m_iInsetL = l; p->m_iInsetT = t; p->m_iInsetR = r; p->m_iInsetB = b; }
        }
        void GetInset(VPANEL vp, int& l, int& t, int& r, int& b) override
        {
            l = t = r = b = 0;
            if (VPanel* p = P(vp)) { l = p->m_iInsetL; t = p->m_iInsetT; r = p->m_iInsetR; b = p->m_iInsetB; }
        }

        void SetVisible(VPANEL vp, bool state) override
        {
            TRACE("IPanel::SetVisible(%lu, %d)\n", vp, (int)state);
            if (VPanel* p = P(vp)) p->SetVisible(state);
        }
        bool IsVisible(VPANEL vp) override { VPanel* p = P(vp); return p ? p->IsVisible() : false; }

        void SetParent(VPANEL vp, VPANEL newParent) override
        {
            TRACE("IPanel::SetParent(%lu, %lu)\n", vp, newParent);
            VPanel* p = P(vp);
            if (!p) return;
            p->SetParent(P(newParent));
        }
        int GetChildCount(VPANEL vp) override { VPanel* p = P(vp); return p ? (int)p->m_Children.size() : 0; }
        VPANEL GetChild(VPANEL vp, int index) override
        {
            VPanel* p = P(vp);
            if (!p || index < 0 || (size_t)index >= p->m_Children.size()) return 0;
            return p->m_Children[(size_t)index]->GetID();
        }
        VPANEL GetParent(VPANEL vp) override
        {
            VPanel* p = P(vp);
            return (p && p->GetParent()) ? p->GetParent()->GetID() : 0;
        }
        void MoveToFront(VPANEL vp) override { Reorder(vp, true); }
        void MoveToBack(VPANEL vp) override { Reorder(vp, false); }
        bool HasParent(VPANEL vp, VPANEL potentialParent) override
        {
            if (vp == potentialParent) return true;
            VPanel* p = P(vp);
            while (p)
            {
                if (p->GetID() == potentialParent) return true;
                p = p->GetParent();
            }
            return false;
        }

        bool IsPopup(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_bPopup : false; }
        void SetPopup(VPANEL vp, bool state) override
        {
            TRACE("IPanel::SetPopup(%lu, %d)\n", vp, (int)state);
            if (VPanel* p = P(vp)) p->m_bPopup = state;
        }
        bool Render_GetPopupVisible(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_bPopupVisible : false; }
        void Render_SetPopupVisible(VPANEL vp, bool state) override { if (VPanel* p = P(vp)) p->m_bPopupVisible = state; }

        HScheme GetScheme(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_hScheme : 0; }
        bool IsProportional(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_bProportional : false; }
        bool IsAutoDeleteSet(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_bAutoDelete : false; }
        void DeletePanel(VPANEL vp) override { TRACE("IPanel::DeletePanel(%lu)\n", vp); }

        void SetKeyBoardInputEnabled(VPANEL vp, bool state) override { if (VPanel* p = P(vp)) p->m_bKeyInput = state; }
        void SetMouseInputEnabled(VPANEL vp, bool state) override { if (VPanel* p = P(vp)) p->m_bMouseInput = state; }
        bool IsKeyBoardInputEnabled(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_bKeyInput : false; }
        bool IsMouseInputEnabled(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_bMouseInput : false; }

        void Solve(VPANEL vp) override {}
        const char* GetName(VPANEL vp) override { VPanel* p = P(vp); return p ? p->GetName() : ""; }
        const char* GetClassName(VPANEL vp) override { VPanel* p = P(vp); return p ? p->GetClassName() : ""; }

        // Traversal / messaging: these are normally forwarded to the IClientPanel
        // (engine-side object). Its vtable is not reproduced yet, so for now we only log.
        void SendMessage(VPANEL vp, void* params, VPANEL from) override { TRACE("IPanel::SendMessage(%lu, %p, %lu)\n", vp, params, from); }
        void Think(VPANEL vp) override { TRACE("IPanel::Think(%lu)\n", vp); }
        void PerformApplySchemeSettings(VPANEL vp) override { TRACE("IPanel::PerformApplySchemeSettings(%lu)\n", vp); }
        void PaintTraverse(VPANEL vp, bool forceRepaint, bool allowForce) override { TRACE("IPanel::PaintTraverse(%lu)\n", vp); }
        void Repaint(VPANEL vp) override { TRACE("IPanel::Repaint(%lu)\n", vp); }
        VPANEL IsWithinTraverse(VPANEL vp, int x, int y, bool traversePopups) override { TRACE("IPanel::IsWithinTraverse(%lu)\n", vp); return 0; }
        void OnChildAdded(VPANEL vp, VPANEL child) override { TRACE("IPanel::OnChildAdded(%lu, %lu)\n", vp, child); }
        void OnSizeChanged(VPANEL vp, int newWide, int newTall) override { TRACE("IPanel::OnSizeChanged(%lu, %d, %d)\n", vp, newWide, newTall); }
        void InternalFocusChanged(VPANEL vp, bool lost) override { TRACE("IPanel::InternalFocusChanged(%lu, %d)\n", vp, (int)lost); }
        bool RequestInfo(VPANEL vp, void* outputData) override { TRACE("IPanel::RequestInfo(%lu)\n", vp); return false; }
        void RequestFocus(VPANEL vp, int direction) override { TRACE("IPanel::RequestFocus(%lu, %d)\n", vp, direction); }
        bool RequestFocusPrev(VPANEL vp, VPANEL existing) override { TRACE("IPanel::RequestFocusPrev(%lu)\n", vp); return false; }
        bool RequestFocusNext(VPANEL vp, VPANEL existing) override { TRACE("IPanel::RequestFocusNext(%lu)\n", vp); return false; }
        VPANEL GetCurrentKeyFocus(VPANEL vp) override { TRACE("IPanel::GetCurrentKeyFocus(%lu)\n", vp); return 0; }
        int GetTabPosition(VPANEL vp) override { TRACE("IPanel::GetTabPosition(%lu)\n", vp); return 0; }

        void* Plat(VPANEL vp) override { VPanel* p = P(vp); return p ? p->m_pPlat : nullptr; }
        void SetPlat(VPANEL vp, void* plat) override { if (VPanel* p = P(vp)) p->m_pPlat = plat; }
        // Returns the Panel* only if the panel belongs to `destinationModule` (e.g. "GameUI"),
        // otherwise NULL - exactly what vgui_controls relies on (it asserts that a parent
        // panel from another module is not treated as its own). Panel derives from
        // IClientPanel in the SDK, so the IClientPanel* given in Init() is also the Panel*.
        // The module is identified by the DLL that contains the client panel's vtable.
        void* GetPanel(VPANEL vp, const char* destinationModule) override
        {
            VPanel* p = P(vp);
            if (!p || !p->m_pClient) return nullptr;
            bool match = destinationModule && p->m_szModule[0] && _stricmp(destinationModule, p->m_szModule) == 0;
            TRACE("IPanel::GetPanel(%lu, '%s') panel module '%s' -> %s\n", vp, destinationModule ? destinationModule : "(null)", p->m_szModule, match ? "match" : "NULL");
            return match ? p->m_pClient : nullptr;
        }
        bool IsEnabled(VPANEL vp) override { VPanel* p = P(vp); return p ? p->IsEnabled() : false; }
        void SetEnabled(VPANEL vp, bool state) override { if (VPanel* p = P(vp)) p->SetEnabled(state); }
        int Slot58(VPANEL vp) override { TRACE("IPanel::Slot58(%lu)\n", vp); return 0; }
        int Slot59(VPANEL vp) override { TRACE("IPanel::Slot59(%lu)\n", vp); return 0; }

    private:
        static VPanel* P(VPANEL vp) { return vp ? GetVPanel((HPanel)vp) : nullptr; }

        void Reorder(VPANEL vp, bool toFront)
        {
            VPanel* p = P(vp);
            if (!p || !p->GetParent()) return;
            auto& v = p->GetParent()->m_Children;
            for (size_t i = 0; i < v.size(); i++)
            {
                if (v[i] == p)
                {
                    v.erase(v.begin() + i);
                    if (toFront) v.push_back(p); else v.insert(v.begin(), p);
                    return;
                }
            }
        }
    };

    static CPanelImpl g_PanelImplInstance;

    void* CreatePanelReal()
    {
        Log("CreatePanelReal() returning %p\n", &g_PanelImplInstance);
        return &g_PanelImplInstance;
    }
}
