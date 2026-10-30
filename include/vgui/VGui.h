#pragma once

// windows.h defines macros (CreateFont -> CreateFontA, PlaySound, SendMessage,
// GetClassName, PostMessage ...) that silently rename our virtual methods.
// Include it here once and remove them, so every file sees the real names.
#ifdef _WIN32
#include <windows.h>
#undef CreateFont
#undef PlaySound
#undef SendMessage
#undef PostMessage
#undef GetClassName
#undef GetCurrentTime
#undef GetTextSize
#endif

#ifdef _WIN32
#ifdef vgui2_EXPORTS
#define VGUI_API __declspec(dllexport)
#else
#define VGUI_API __declspec(dllimport)
#endif
#else
#define VGUI_API
#endif

namespace vgui
{
    typedef unsigned long VPANEL;
    typedef unsigned long HPanel;
    typedef unsigned long HScheme;
    typedef unsigned long HFont;
    typedef unsigned long HCursor;
    typedef unsigned long HTexture;
    
    const HPanel INVALID_PANEL = 0xffffffff;
    const HFont INVALID_FONT = 0;
    
    class IPanel;
    class ISurface;
    class IVGui;
    class ILocalize;
    class IInput;
    class ISystem;
    class IFileSystem;
    class IKeyValues;
    
    typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
    
    struct InterfaceReg
    {
        void* (*m_CreateFn)();
        const char* m_pName;
        InterfaceReg* m_pNext;
        
        InterfaceReg(void* (*createFn)(), const char* name)
            : m_CreateFn(createFn), m_pName(name), m_pNext(nullptr)
        {
        }
    };
    
    extern "C" VGUI_API void* CreateInterface(const char* pName, int* pReturnCode);
    
    void InitializeInterfaces(CreateInterfaceFn* pFactoryList, int nFactoryCount);
    
    void RegisterInterface(InterfaceReg* pReg);
}
