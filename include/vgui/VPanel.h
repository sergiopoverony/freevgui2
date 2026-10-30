#pragma once
#include "VGui.h"
#include <cstring>
#include <vector>

namespace vgui
{
    class VPanel
    {
    public:
        VPanel();
        virtual ~VPanel();
        
        void Init();
        void SetName(const char* name);
        const char* GetName() const;
        void SetClassName(const char* className);
        const char* GetClassName() const;
        
        void SetPos(int x, int y);
        void GetPos(int& x, int& y) const;
        void SetSize(int wide, int tall);
        void GetSize(int& wide, int& tall) const;
        
        void SetVisible(bool state);
        bool IsVisible() const;
        void SetEnabled(bool state);
        bool IsEnabled() const;
        
        void SetParent(VPanel* parent);
        VPanel* GetParent() const;
        
        HPanel GetID() const { return m_nID; }
        void SetID(HPanel id) { m_nID = id; }
        
        // Extra state used by IPanel (public on purpose: plain data holder)
        void* m_pClient = nullptr;       // IClientPanel* handed over in IPanel::Init
        char m_szModule[64] = {0};       // DLL base name (no extension) that owns the client panel, e.g. "GameUI"
        void* m_pPlat = nullptr;         // SurfacePlat* (IPanel::Plat/SetPlat)
        int m_iMinWide = 0, m_iMinTall = 0, m_iZPos = 0;
        int m_iInsetL = 0, m_iInsetT = 0, m_iInsetR = 0, m_iInsetB = 0;
        bool m_bPopup = false, m_bPopupVisible = true;
        bool m_bKeyInput = true, m_bMouseInput = true;
        bool m_bProportional = false, m_bAutoDelete = false;
        HScheme m_hScheme = 0;
        std::vector<VPanel*> m_Children;

    private:
        HPanel m_nID;
        char* m_pszName;
        char* m_pszClassName;
        int m_iX, m_iY;
        int m_iWide, m_iTall;
        bool m_bVisible;
        bool m_bEnabled;
        VPanel* m_pParent;
    };
}
