#include <windows.h>
#include <cstring>
#include <map>
#include <vector>
#include <cstdio>
#include "vgui/VGui.h"
#include "vgui/IPanel.h"
#include "vgui/IInput.h"
#include "vgui/IInputInternal.h"
#include "vgui/IVGui.h"
#include "vgui/IFileSystem.h"
#include "vgui/ILocalize.h"
#include "vgui/IKeyValues.h"
#include "vgui/ISurface.h"
#include "vgui/ISchemeManager.h"
#include "vgui/ISystem.h"
#include "vgui/VPanel.h"

namespace vgui
{
    static FILE* g_pLogFile = nullptr;
    
    void Log(const char* format, ...)
    {
        if (!g_pLogFile)
        {
            g_pLogFile = fopen("vgui2_debug.log", "w");
            if (!g_pLogFile) return;
        }
        
        va_list args;
        va_start(args, format);
        vfprintf(g_pLogFile, format, args);
        va_end(args);
        fflush(g_pLogFile);
    }
    
    static InterfaceReg* g_pInterfaceRegs = nullptr;

    IFileSystem* g_pFileSystem = nullptr;
    IPanel* g_pPanel = nullptr;
    ILocalize* g_pLocalize = nullptr;
    IKeyValues* g_pKeyValues = nullptr;
    IInput* g_pInput = nullptr;

    static std::map<HPanel, VPanel*> g_VPanelMap;
    static HPanel g_nNextPanelID = 1;

    extern void* CreateSchemeManager();
    extern void* CreateLocalizeReal();
    extern void* CreateLocalize002Real();
    extern void* CreateKeyValuesReal();
    extern void* CreateSurfaceReal();
    extern void* CreateInputReal();
    extern void* CreateSystemReal();
    extern void* CreateInputInternalReal();
    extern void* CreateVGuiReal();
    extern void* CreatePanelReal();

    static void* CreateInput() { Log("CreateInput() called\n"); return CreateInputReal(); }
    static void* CreateLocalize002() { Log("CreateLocalize002() called\n"); return CreateLocalize002Real(); }
    static void* CreateLocalize() { Log("CreateLocalize() called\n"); return CreateLocalizeReal(); }
    static void* CreateKeyValues() { Log("CreateKeyValues() called\n"); return CreateKeyValuesReal(); }
    static void* CreateSurface() { Log("CreateSurface() called\n"); return CreateSurfaceReal(); }
    static void* CreatePanel() { Log("CreatePanel() called\n"); return CreatePanelReal(); }
    static void* CreateSystem() { Log("CreateSystem() called\n"); return CreateSystemReal(); }
    static void* CreateInputInternal() { Log("CreateInputInternal() called\n"); return CreateInputInternalReal(); }
    static void* CreateVGui() { Log("CreateVGui() called\n"); return CreateVGuiReal(); }

    static InterfaceReg g_PanelReg(CreatePanel, VGUI_PANEL_INTERFACE_VERSION);
    static InterfaceReg g_InputReg(CreateInput, VGUI_INPUT_INTERFACE_VERSION);
    static InterfaceReg g_InputInternalReg(CreateInputInternal, VGUI_INPUTINTERNAL_INTERFACE_VERSION);
    static InterfaceReg g_VGuiReg(CreateVGui, VGUI_IVGUI_INTERFACE_VERSION);
    // NOTE: the original vgui2.dll does NOT provide "VFileSystem009" (it only queries it from
    // the engine's factories), so it must not be registered here. It does provide Surface026
    // and both Localize002/Localize003. Registered set = the 10 interfaces of the original.
    static InterfaceReg g_LocalizeReg(CreateLocalize, VGUI_LOCALIZE_INTERFACE_VERSION);
    static InterfaceReg g_Localize002Reg(CreateLocalize002, "VGUI_Localize002");
    static InterfaceReg g_KeyValuesReg(CreateKeyValues, KEYVALUES_INTERFACE_VERSION);
    static InterfaceReg g_SurfaceReg(CreateSurface, "VGUI_Surface026");
    static InterfaceReg g_SchemeManagerReg(CreateSchemeManager, "VGUI_Scheme009");
    static InterfaceReg g_SystemReg(CreateSystem, VGUI_SYSTEM_INTERFACE_VERSION);

    void RegisterInterface(InterfaceReg* pReg)
    {
        if (!pReg) return;
        pReg->m_pNext = g_pInterfaceRegs;
        g_pInterfaceRegs = pReg;
    }

    extern "C" VGUI_API void* CreateInterface(const char* pName, int* pReturnCode)
    {
        Log("CreateInterface('%s') called\n", pName ? pName : "null");
        
        InterfaceReg* pCur = g_pInterfaceRegs;
        while (pCur != nullptr)
        {
            const char* pRegName = pCur->m_pName;
            const char* pQueryName = pName;
            while (*pRegName == *pQueryName)
            {
                if (*pRegName == 0)
                {
                    Log("  -> Found: %s\n", pRegName);
                    if (pReturnCode) *pReturnCode = 0;
                    return pCur->m_CreateFn();
                }
                pRegName++;
                pQueryName++;
            }
            pCur = pCur->m_pNext;
        }
        
        Log("  -> NOT FOUND: %s\n", pName ? pName : "null");
        if (pReturnCode) *pReturnCode = 1;
        return nullptr;
    }

    VGUI_API HPanel AllocatePanel(const char* className, HPanel parentId)
    {
        VPanel* pPanel = new VPanel();
        HPanel id = g_nNextPanelID++;
        pPanel->SetID(id);
        if (className) pPanel->SetClassName(className);
        pPanel->Init();

        if (parentId != INVALID_PANEL)
        {
            auto it = g_VPanelMap.find(parentId);
            if (it != g_VPanelMap.end())
            {
                pPanel->SetParent(it->second);
            }
        }

        g_VPanelMap[id] = pPanel;
        return id;
    }

    VGUI_API void FreePanel(HPanel panel)
    {
        auto it = g_VPanelMap.find(panel);
        if (it != g_VPanelMap.end())
        {
            delete it->second;
            g_VPanelMap.erase(it);
        }
    }

    VGUI_API VPanel* GetVPanel(HPanel panel)
    {
        auto it = g_VPanelMap.find(panel);
        if (it != g_VPanelMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void InitializeInterfaces(CreateInterfaceFn* pFactoryList, int nFactoryCount)
    {
        Log("InitializeInterfaces() called with %d factories\n", nFactoryCount);
        
        if (!pFactoryList || nFactoryCount <= 0) return;
        
        for (int i = 0; i < nFactoryCount; i++)
        {
            if (!pFactoryList[i]) continue;
            void* p;
            
            p = pFactoryList[i]("VFileSystem009", nullptr);
            if (p) g_pFileSystem = static_cast<IFileSystem*>(p);
            
            p = pFactoryList[i]("VGUI_Panel007", nullptr);
            if (p) g_pPanel = static_cast<IPanel*>(p);
            
            p = pFactoryList[i]("KeyValues003", nullptr);
            if (p) g_pKeyValues = static_cast<IKeyValues*>(p);
            
            p = pFactoryList[i]("VGUI_Localize003", nullptr);
            if (p) g_pLocalize = static_cast<ILocalize*>(p);
            
            p = pFactoryList[i]("VGUI_Input004", nullptr);
            if (p) g_pInput = static_cast<IInput*>(p);
        }
    }
}


// ---------------------------------------------------------------------------
// Crash logger: first-chance logging of fatal exceptions (access violation etc.)
// with module+RVA of the faulting address and of return addresses on the stack.
// Output goes to vgui2_debug.log. Remove once the vtables are all correct.
// ---------------------------------------------------------------------------
static const char* DescribeAddr(DWORD addr, char* out, size_t outSize, bool* pInModule)
{
    HMODULE hMod = nullptr;
    char path[MAX_PATH] = {0};
    bool ok = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                 (LPCSTR)(ULONG_PTR)addr, &hMod) != 0;
    if (pInModule) *pInModule = ok;
    if (ok && GetModuleFileNameA(hMod, path, MAX_PATH))
    {
        const char* name = strrchr(path, '\\');
        name = name ? name + 1 : path;
        _snprintf(out, outSize, "%s+0x%X (base %08X)", name, (unsigned)(addr - (DWORD)(ULONG_PTR)hMod), (unsigned)(ULONG_PTR)hMod);
    }
    else
    {
        _snprintf(out, outSize, "<no module>");
    }
    out[outSize - 1] = 0;
    return out;
}

static void LogStack(DWORD esp)
{
    __try
    {
        const DWORD* p = (const DWORD*)(ULONG_PTR)esp;
        int shown = 0;
        for (int i = 0; i < 96 && shown < 24; i++)
        {
            DWORD v = p[i];
            bool inMod = false;
            char desc[300];
            DescribeAddr(v, desc, sizeof(desc), &inMod);
            if (inMod)
            {
                vgui::Log("    [esp+%02X] %08X  %s\n", i * 4, v, desc);
                shown++;
            }
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        vgui::Log("    (stack unreadable)\n");
    }
}

static LONG CALLBACK CrashLogger(PEXCEPTION_POINTERS ep)
{
    DWORD code = ep->ExceptionRecord->ExceptionCode;
    if ((code & 0xF0000000) != 0xC0000000)
        return EXCEPTION_CONTINUE_SEARCH;

    static volatile LONG s_count = 0;
    if (InterlockedIncrement(&s_count) > 8)
        return EXCEPTION_CONTINUE_SEARCH;

    char desc[300];
    PCONTEXT c = ep->ContextRecord;
    vgui::Log("\n!!! EXCEPTION 0x%08X at %08X  %s\n", code, (unsigned)(ULONG_PTR)ep->ExceptionRecord->ExceptionAddress,
              DescribeAddr((DWORD)(ULONG_PTR)ep->ExceptionRecord->ExceptionAddress, desc, sizeof(desc), nullptr));
    if (code == EXCEPTION_ACCESS_VIOLATION && ep->ExceptionRecord->NumberParameters >= 2)
    {
        vgui::Log("    access violation: %s address %08X\n",
                  ep->ExceptionRecord->ExceptionInformation[0] == 0 ? "READ" :
                  ep->ExceptionRecord->ExceptionInformation[0] == 1 ? "WRITE" : "EXEC/DEP",
                  (unsigned)ep->ExceptionRecord->ExceptionInformation[1]);
    }
#ifdef _M_IX86
    vgui::Log("    EIP=%08X ESP=%08X EBP=%08X EAX=%08X ECX=%08X EDX=%08X EBX=%08X ESI=%08X EDI=%08X\n",
              c->Eip, c->Esp, c->Ebp, c->Eax, c->Ecx, c->Edx, c->Ebx, c->Esi, c->Edi);
    LogStack(c->Esp);
#endif
    return EXCEPTION_CONTINUE_SEARCH;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        AddVectoredExceptionHandler(1, CrashLogger);
        vgui::Log("=== VGUI2 DLL Loaded ===\n");
        vgui::RegisterInterface(&vgui::g_PanelReg);
        vgui::RegisterInterface(&vgui::g_InputReg);
        vgui::RegisterInterface(&vgui::g_InputInternalReg);
        vgui::RegisterInterface(&vgui::g_VGuiReg);
        vgui::RegisterInterface(&vgui::g_LocalizeReg);
        vgui::RegisterInterface(&vgui::g_Localize002Reg);
        vgui::RegisterInterface(&vgui::g_KeyValuesReg);
        vgui::RegisterInterface(&vgui::g_SurfaceReg);
        vgui::RegisterInterface(&vgui::g_SchemeManagerReg);
        vgui::RegisterInterface(&vgui::g_SystemReg);
        break;
    case DLL_PROCESS_DETACH:
        vgui::Log("=== VGUI2 DLL Unloaded ===\n");
        if (vgui::g_pLogFile)
        {
            fclose(vgui::g_pLogFile);
            vgui::g_pLogFile = nullptr;
        }
        break;
    }
    return TRUE;
}
