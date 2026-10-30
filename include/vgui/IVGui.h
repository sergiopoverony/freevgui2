#pragma once
#include "VGui.h"

namespace vgui
{
    class IVGui
    {
    public:
        virtual ~IVGui() {}
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual bool IsRunning() = 0;
        virtual void RunFrame() = 0;
        virtual void RegisterPanel(HPanel panel) = 0;
        virtual void DeregisterPanel(HPanel panel) = 0;
        virtual void Sleep(unsigned int milliseconds) = 0;
        virtual void AddTickSignal(HPanel panel, unsigned int interval) = 0;
        virtual void RemoveTickSignal(HPanel panel) = 0;
    };
    
    #define VGUI_IVGUI_INTERFACE_VERSION "VGUI_ivgui006"
}
