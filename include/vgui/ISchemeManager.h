#pragma once
#include "VGui.h"

namespace vgui
{
    class ISchemeManager
    {
    public:
        virtual ~ISchemeManager() {}
        
        virtual HScheme GetScheme(const char* schemeName, bool proportional) = 0;
        virtual const char* GetColor(HScheme scheme, const char* colorName) = 0;
        virtual HFont GetFont(HScheme scheme, const char* fontName) = 0;
        virtual void ReloadSchemes() = 0;
        virtual void AddCustomFont(const char* fontName) = 0;
    };
    
    #define VGUI_SCHEMEMANAGER_INTERFACE_VERSION "VGUI_Scheme010"
}
