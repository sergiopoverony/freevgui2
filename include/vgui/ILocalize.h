#pragma once
#include "VGui.h"

namespace vgui
{
    class ILocalize
    {
    public:
        virtual bool AddFile(const char* fileName) = 0;
        virtual void RemoveAll() = 0;
        virtual const wchar_t* Find(const char* tokenName) = 0;
    };
    
    #define VGUI_LOCALIZE_INTERFACE_VERSION "VGUI_Localize003"
}