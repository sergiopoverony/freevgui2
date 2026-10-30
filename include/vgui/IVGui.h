#pragma once
#include "VGui.h"

// windows.h defines PostMessage -> PostMessageA, which renames our virtual method
#ifdef PostMessage
#undef PostMessage
#endif

namespace vgui
{
    typedef unsigned long VPANEL;
    typedef unsigned long HContext;

    // Order of virtual methods MUST match the original VGUI_ivgui006 vtable
    // (GoldSrc vgui2.dll). There is NO virtual destructor in the real interface
    // (IBaseInterface has a non-virtual dtor), and Init takes the factory list.
    // On x86 __thiscall the callee pops its own arguments, so a wrong
    // signature corrupts ESP and crashes the caller right after the call.
    class IVGui
    {
    public:
        virtual bool Init(CreateInterfaceFn* factoryList, int numFactories) = 0;   // 0
        virtual void Shutdown() = 0;                                                // 1
        virtual void Start() = 0;                                                   // 2
        virtual void Stop() = 0;                                                    // 3
        virtual bool IsRunning() = 0;                                               // 4
        virtual void RunFrame() = 0;                                                // 5
        virtual void ShutdownMessage(unsigned int shutdownID) = 0;                  // 6
        virtual VPANEL AllocPanel() = 0;                                            // 7
        virtual void FreePanel(VPANEL panel) = 0;                                   // 8
        virtual void DPrintf(const char* format, ...) = 0;                          // 9
        virtual void DPrintf2(const char* format, ...) = 0;                         // 10
        virtual void SpewAllActivePanelNames() = 0;                                 // 11
        virtual HPanel PanelToHandle(VPANEL panel) = 0;                             // 12
        virtual VPANEL HandleToPanel(HPanel index) = 0;                             // 13
        virtual void MarkPanelForDeletion(VPANEL panel) = 0;                        // 14
        virtual void AddTickSignal(VPANEL panel, int intervalMilliseconds = 0) = 0; // 15
        virtual void RemoveTickSignal(VPANEL panel) = 0;                            // 16
        virtual void PostMessage(VPANEL target, void* params, VPANEL from, float delaySeconds = 0.0f) = 0; // 17
        virtual HContext CreateContext() = 0;                                       // 18
        virtual void DestroyContext(HContext context) = 0;                          // 19
        virtual void AssociatePanelWithContext(HContext context, VPANEL pRoot) = 0; // 20
        virtual void ActivateContext(HContext context) = 0;                         // 21
        virtual void SetSleep(bool state) = 0;                                      // 22
        virtual bool GetShouldVGuiControlSleep() = 0;                               // 23
    };

    #define VGUI_IVGUI_INTERFACE_VERSION "VGUI_ivgui006"
}
