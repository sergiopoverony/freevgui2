#include "vgui/IVGui.h"
#include <windows.h>
#undef PostMessage
#include <cstdio>
#include <cstdarg>

namespace vgui
{
    extern void Log(const char* format, ...);
    VGUI_API HPanel AllocatePanel(const char* className, HPanel parentId);   // dllmain.cpp
    VGUI_API void FreePanel(HPanel panel);                                  // dllmain.cpp

    static VPANEL AllocatePanelHandle() { return AllocatePanel("Panel", INVALID_PANEL); }
    static void FreePanelHandle(VPANEL panel) { FreePanel(panel); }

    static CreateInterfaceFn g_Factories[16] = {};
    static int g_nFactories = 0;

    class CVGuiImpl : public IVGui
    {
    public:
        CVGuiImpl() : m_bRunning(false), m_bSleep(true), m_nNextContext(1) {}

        bool Init(CreateInterfaceFn* factoryList, int numFactories) override
        {
            Log("IVGui::Init(list=%p, n=%d)\n", (void*)factoryList, numFactories);
            g_nFactories = 0;
            if (factoryList)
            {
                for (int i = 0; i < numFactories && i < 16; i++)
                    g_Factories[g_nFactories++] = factoryList[i];
            }
            InitializeInterfaces(g_Factories, g_nFactories);
            return true;
        }

        void Shutdown() override            { Log("IVGui::Shutdown\n"); m_bRunning = false; }
        void Start() override               { Log("IVGui::Start\n");    m_bRunning = true; }
        void Stop() override                { Log("IVGui::Stop\n");     m_bRunning = false; }
        bool IsRunning() override           { return m_bRunning; }
        void RunFrame() override            {}
        void ShutdownMessage(unsigned int shutdownID) override { Log("IVGui::ShutdownMessage(%u)\n", shutdownID); }

        VPANEL AllocPanel() override        { return AllocatePanelHandle(); }
        void FreePanel(VPANEL panel) override { FreePanelHandle(panel); }

        void DPrintf(const char* format, ...) override
        {
            char buf[1024];
            va_list args; va_start(args, format);
            _vsnprintf(buf, sizeof(buf) - 1, format, args);
            va_end(args);
            buf[sizeof(buf) - 1] = 0;
            Log("DPrintf: %s", buf);
        }
        void DPrintf2(const char* format, ...) override
        {
            char buf[1024];
            va_list args; va_start(args, format);
            _vsnprintf(buf, sizeof(buf) - 1, format, args);
            va_end(args);
            buf[sizeof(buf) - 1] = 0;
            Log("DPrintf2: %s", buf);
        }
        void SpewAllActivePanelNames() override {}

        HPanel PanelToHandle(VPANEL panel) override { return panel; }
        VPANEL HandleToPanel(HPanel index) override { return index; }
        void MarkPanelForDeletion(VPANEL panel) override { FreePanelHandle(panel); }

        void AddTickSignal(VPANEL panel, int interval) override { Log("IVGui::AddTickSignal(%lu,%d)\n", panel, interval); }
        void RemoveTickSignal(VPANEL panel) override            { Log("IVGui::RemoveTickSignal(%lu)\n", panel); }
        void PostMessage(VPANEL target, void* params, VPANEL from, float delay) override {}

        HContext CreateContext() override                       { return m_nNextContext++; }
        void DestroyContext(HContext) override                  {}
        void AssociatePanelWithContext(HContext, VPANEL) override {}
        void ActivateContext(HContext) override                 {}
        void SetSleep(bool state) override                      { m_bSleep = state; }
        bool GetShouldVGuiControlSleep() override               { return m_bSleep; }

    private:
        bool m_bRunning;
        bool m_bSleep;
        HContext m_nNextContext;
    };

    static CVGuiImpl g_VGuiImplInstance;

    void* CreateVGuiReal()
    {
        Log("CreateVGuiReal() returning %p\n", &g_VGuiImplInstance);
        return &g_VGuiImplInstance;
    }
}
