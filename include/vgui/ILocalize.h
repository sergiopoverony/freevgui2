#pragma once
#include "VGui.h"

namespace vgui
{
    typedef unsigned int StringIndex_t;
    const StringIndex_t INVALID_LOCALIZE_STRING_INDEX = 0xFFFFFFFF;

    // VGUI_Localize003: 21 slots, argument counts verified against the original
    // (original/vtable_signatures.txt). Names follow the Source SDK; slots 9 and 20
    // differ from the SDK and are unnamed. Overloads have distinct names.
    class ILocalize
    {
    public:
        virtual ~ILocalize() {}                                                                                   // 0
        virtual bool AddFile(void* fileSystem, const char* fileName) = 0;                                         // 1
        virtual void RemoveAll() = 0;                                                                             // 2
        virtual const wchar_t* Find(const char* tokenName) = 0;                                                   // 3
        virtual int ConvertANSIToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes) = 0;   // 4
        virtual int ConvertUnicodeToANSI(const wchar_t* unicode, char* ansi, int ansiBufferSize) = 0;             // 5
        virtual StringIndex_t FindIndex(const char* tokenName) = 0;                                               // 6
        virtual void ConstructStringFmt(wchar_t* out, int outBytes, const wchar_t* format, void* variables) = 0;  // 7
        virtual void ConstructStringIdx(wchar_t* out, int outBytes, StringIndex_t index, void* variables) = 0;    // 8
        virtual int Slot9() = 0;                                                                                  // 9
        virtual const char* GetNameByIndex(StringIndex_t index) = 0;                                              // 10
        virtual const wchar_t* GetValueByIndex(StringIndex_t index) = 0;                                          // 11
        virtual StringIndex_t GetFirstStringIndex() = 0;                                                          // 12
        virtual StringIndex_t GetNextStringIndex(StringIndex_t index) = 0;                                        // 13
        virtual void AddString(const char* tokenName, const wchar_t* unicodeString, const char* fileName) = 0;    // 14
        virtual void SetValueByIndex(StringIndex_t index, const wchar_t* newValue) = 0;                           // 15
        virtual bool SaveToFile(void* fileSystem, const char* fileName) = 0;                                      // 16
        virtual int GetLocalizationFileCount() = 0;                                                               // 17
        virtual const char* GetLocalizationFileName(int index) = 0;                                               // 18
        virtual const char* GetFileNameByIndex(StringIndex_t index) = 0;                                          // 19
        virtual int Slot20(int a1) = 0;                                                                           // 20
    };

    #define VGUI_LOCALIZE_INTERFACE_VERSION "VGUI_Localize003"
}
