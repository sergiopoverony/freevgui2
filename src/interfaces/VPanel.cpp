#include "vgui/VPanel.h"
#include <cstdlib>
#include <cstring>

namespace vgui
{
    VPanel::VPanel()
        : m_nID(INVALID_PANEL)
        , m_pszName(nullptr)
        , m_pszClassName(nullptr)
        , m_iX(0), m_iY(0)
        , m_iWide(0), m_iTall(0)
        , m_bVisible(true)
        , m_bEnabled(true)
        , m_pParent(nullptr)
    {
    }
    
    VPanel::~VPanel()
    {
        if (m_pszName) free(m_pszName);
        if (m_pszClassName) free(m_pszClassName);
    }
    
    void VPanel::Init()
    {
        m_nID = INVALID_PANEL;
        m_iX = m_iY = 0;
        m_iWide = m_iTall = 0;
        m_bVisible = true;
        m_bEnabled = true;
        m_pParent = nullptr;
    }
    
    void VPanel::SetName(const char* name)
    {
        if (m_pszName) free(m_pszName);
        if (name)
        {
            size_t len = strlen(name) + 1;
            m_pszName = (char*)malloc(len);
            if (m_pszName) strcpy(m_pszName, name);
        }
        else
        {
            m_pszName = nullptr;
        }
    }
    
    const char* VPanel::GetName() const
    {
        return m_pszName ? m_pszName : "";
    }
    
    void VPanel::SetClassName(const char* className)
    {
        if (m_pszClassName) free(m_pszClassName);
        if (className)
        {
            size_t len = strlen(className) + 1;
            m_pszClassName = (char*)malloc(len);
            if (m_pszClassName) strcpy(m_pszClassName, className);
        }
        else
        {
            m_pszClassName = nullptr;
        }
    }
    
    const char* VPanel::GetClassName() const
    {
        return m_pszClassName ? m_pszClassName : "";
    }
    
    void VPanel::SetPos(int x, int y)
    {
        m_iX = x;
        m_iY = y;
    }
    
    void VPanel::GetPos(int& x, int& y) const
    {
        x = m_iX;
        y = m_iY;
    }
    
    void VPanel::SetSize(int wide, int tall)
    {
        m_iWide = wide;
        m_iTall = tall;
    }
    
    void VPanel::GetSize(int& wide, int& tall) const
    {
        wide = m_iWide;
        tall = m_iTall;
    }
    
    void VPanel::SetVisible(bool state)
    {
        m_bVisible = state;
    }
    
    bool VPanel::IsVisible() const
    {
        return m_bVisible;
    }
    
    void VPanel::SetEnabled(bool state)
    {
        m_bEnabled = state;
    }
    
    bool VPanel::IsEnabled() const
    {
        return m_bEnabled;
    }
    
    void VPanel::SetParent(VPanel* parent)
    {
        m_pParent = parent;
    }
    
    VPanel* VPanel::GetParent() const
    {
        return m_pParent;
    }
}
