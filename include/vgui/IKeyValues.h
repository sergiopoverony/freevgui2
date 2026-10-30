#pragma once
#include "VGui.h"

namespace vgui
{
    class IKeyValues
    {
    public:
        virtual void* CreateKeyValues(const char* name) = 0;
        virtual void DeleteKeyValues(void* kv) = 0;
    };
    
    #define KEYVALUES_INTERFACE_VERSION "KeyValues003"
}