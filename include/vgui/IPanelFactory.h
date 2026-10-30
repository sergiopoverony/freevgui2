#pragma once
#include "VGui.h"

namespace vgui
{
    class IPanelFactory
    {
    public:
        virtual ~IPanelFactory() {}
        virtual HPanel CreatePanel(const char* className, HPanel parentId) = 0;
        virtual const char* GetClassName() = 0;
    };
    
    class IPanelFactoryManager
    {
    public:
        virtual ~IPanelFactoryManager() {}
        virtual void RegisterFactory(IPanelFactory* factory) = 0;
        virtual HPanel CreatePanel(const char* className, HPanel parentId) = 0;
        virtual void UnregisterFactory(const char* className) = 0;
    };
}
