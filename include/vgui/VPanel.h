#pragma once
#include "VGui.h"
#include <cstring>

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
