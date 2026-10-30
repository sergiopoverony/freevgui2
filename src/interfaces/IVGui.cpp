#include "vgui/IVGui.h"
#include <windows.h>
#include <cstdio>

namespace vgui
{
    extern void Log(const char* format, ...);
    
    class CVGuiImpl : public IVGui
    {
    public:
        CVGuiImpl() : m_bRunning(false) { Log("CVGuiImpl::CVGuiImpl() this=%p\n", this); }
        ~CVGuiImpl() { Log("CVGuiImpl::~CVGuiImpl() this=%p\n", this); }
        
        void Init() override 
        { 
            __try
            {
                Log("CVGuiImpl::Init() START this=%p\n", this);
                Log("CVGuiImpl::Init() END\n");
            }
            __except(EXCEPTION_EXECUTE_HANDLER)
            {
                Log("CVGuiImpl::Init() EXCEPTION: 0x%X\n", GetExceptionCode());
            }
        }
        
        void Shutdown() override 
        { 
            __try
            {
                Log("CVGuiImpl::Shutdown() START\n");
                m_bRunning = false; 
                Log("CVGuiImpl::Shutdown() END\n");
            }
            __except(EXCEPTION_EXECUTE_HANDLER)
            {
                Log("CVGuiImpl::Shutdown() EXCEPTION: 0x%X\n", GetExceptionCode());
            }
        }
        
        void Start() override 
        { 
            __try
            {
                Log("CVGuiImpl::Start() START\n");
                m_bRunning = true; 
                Log("CVGuiImpl::Start() END\n");
            }
            __except(EXCEPTION_EXECUTE_HANDLER)
            {
                Log("CVGuiImpl::Start() EXCEPTION: 0x%X\n", GetExceptionCode());
            }
        }
        
        void Stop() override 
        { 
            __try
            {
                Log("CVGuiImpl::Stop() START\n");
                m_bRunning = false; 
                Log("CVGuiImpl::Stop() END\n");
            }
            __except(EXCEPTION_EXECUTE_HANDLER)
            {
                Log("CVGuiImpl::Stop() EXCEPTION: 0x%X\n", GetExceptionCode());
            }
        }
        
        bool IsRunning() override
        {
            Log("CVGuiImpl::IsRunning() -> %d\n", m_bRunning);
            return m_bRunning;
        }
        
        void RunFrame() override 
        { 
            Log("CVGuiImpl::RunFrame()\n");
        }
        
        void RegisterPanel(HPanel panel) override 
        { 
            Log("CVGuiImpl::RegisterPanel(%u)\n", panel);
        }
        
        void DeregisterPanel(HPanel panel) override 
        { 
            Log("CVGuiImpl::DeregisterPanel(%u)\n", panel);
        }
        
        void Sleep(unsigned int milliseconds) override
        {
            Log("CVGuiImpl::Sleep(%u)\n", milliseconds);
            ::Sleep(milliseconds);
        }
        
        void AddTickSignal(HPanel panel, unsigned int interval) override 
        { 
            Log("CVGuiImpl::AddTickSignal(%u, %u)\n", panel, interval);
        }
        
        void RemoveTickSignal(HPanel panel) override 
        { 
            Log("CVGuiImpl::RemoveTickSignal(%u)\n", panel);
        }
        
    private:
        bool m_bRunning;
    };
    
    static CVGuiImpl g_VGuiImplInstance;
    
    void* CreateVGuiReal()
    {
        Log("CreateVGuiReal() returning %p\n", &g_VGuiImplInstance);
        return &g_VGuiImplInstance;
    }
}
