#pragma once
#include "VGui.h"

namespace vgui
{
    typedef int HKeySymbol;

    // "KeyValues003" is NOT a KeyValues factory. The KeyValues class itself is
    // compiled into the engine; vgui2.dll only provides the KeyValues *system*
    // (IKeyValuesSystem): memory allocation and the string<->symbol table.
    //
    // Evidence: the engine calls vtable slot 1 right after startup with the
    // argument 24 (sizeof(KeyValues)) -> RegisterSizeofKeyValues(24). Slot 0
    // is the virtual destructor (same as IVGui).
    //
    // The order of virtual methods MUST NOT change.
    class IKeyValues
    {
    public:
        virtual ~IKeyValues() {}                                                              // 0
        virtual void RegisterSizeofKeyValues(int size) = 0;                                   // 1
        virtual void* AllocKeyValuesMemory(int size) = 0;                                     // 2
        virtual void FreeKeyValuesMemory(void* pMem) = 0;                                     // 3
        virtual HKeySymbol GetSymbolForString(const char* name) = 0;                          // 4
        virtual const char* GetStringForSymbol(HKeySymbol symbol) = 0;                        // 5
        virtual void GetLocalizedFromANSI(const char* ansi, wchar_t* outBuf, int unicodeBufferSizeInBytes) = 0; // 6
        virtual void GetANSIFromLocalized(const wchar_t* wchar, char* outBuf, int ansiBufferSizeInBytes) = 0;   // 7
        virtual void AddKeyValuesToMemoryLeakList(void* pMem, HKeySymbol name) = 0;           // 8
        virtual void RemoveKeyValuesFromMemoryLeakList(void* pMem) = 0;                       // 9
    };

    #define KEYVALUES_INTERFACE_VERSION "KeyValues003"
}
