#pragma once
#include "VGui.h"

namespace vgui
{
    // VGUI_System009: 34 slots, argument counts verified against the original
    // (original/vtable_signatures.txt). Overloads have distinct names (...A / ...W)
    // because MSVC reorders overloaded virtuals in the vtable.
    class ISystem
    {
    public:
        virtual ~ISystem() {}                                                                    // 0
        virtual void Shutdown() = 0;                                                             // 1
        virtual void RunFrame() = 0;                                                             // 2
        virtual void ShellExecute(const char* command, const char* file) = 0;                    // 3
        virtual double GetFrameTime() = 0;                                                       // 4
        virtual double GetCurrentTime() = 0;                                                     // 5
        virtual long GetTimeMillis() = 0;                                                        // 6
        virtual int GetClipboardTextCount() = 0;                                                 // 7
        virtual void SetClipboardTextA(const char* text, int textLen) = 0;                       // 8
        virtual void SetClipboardTextW(const wchar_t* text, int textLen) = 0;                    // 9
        virtual int GetClipboardTextA(int offset, char* buf, int bufLen) = 0;                    // 10
        virtual int GetClipboardTextW(int offset, wchar_t* buf, int bufLen) = 0;                 // 11
        virtual bool SetRegistryString(const char* key, const char* value) = 0;                  // 12
        virtual bool GetRegistryString(const char* key, char* value, int valueLen) = 0;          // 13
        virtual bool SetRegistryInteger(const char* key, int value) = 0;                         // 14
        virtual bool GetRegistryInteger(const char* key, int& value) = 0;                        // 15
        virtual void* GetUserConfigFileData(const char* dialogName, int dialogID) = 0;           // 16
        virtual void SetUserConfigFile(const char* fileName, const char* pathName) = 0;          // 17
        virtual void SaveUserConfigFile() = 0;                                                   // 18
        virtual bool SetWatchForComputerUse(bool state) = 0;                                     // 19
        virtual double GetTimeSinceLastUse() = 0;                                                // 20
        virtual int GetAvailableDrives(char* buf, int bufLen) = 0;                               // 21
        virtual bool CommandLineParamExists(const char* paramName) = 0;                          // 22
        virtual int Slot23(int a1, int a2) = 0;                                                  // 23
        virtual bool GetCurrentTimeAndDate(int* year, int* month, int* dayOfWeek, int* day, int* hour, int* minute, int* second) = 0; // 24
        virtual double GetFreeDiskSpace(const char* path) = 0;                                   // 25
        virtual bool CreateShortcut(const char* linkFileName, const char* targetPath, const char* arguments, const char* workingDirectory, const char* iconFile) = 0; // 26
        virtual bool GetShortcutTarget(const char* linkFileName, char* targetPath, char* arguments, int destBufferSizes) = 0; // 27
        virtual bool ModifyShortcutTarget(const char* linkFileName, const char* targetPath, const char* arguments, const char* workingDirectory) = 0; // 28
        virtual bool GetCommandLineParamValue(const char* paramName, char* value, int valueBufferSize) = 0; // 29
        virtual bool DeleteRegistryKey(const char* keyName) = 0;                                 // 30
        virtual const char* GetDesktopFolderPath() = 0;                                          // 31
        virtual int Slot32(int a1) = 0;                                                          // 32
        virtual int Slot33(int a1) = 0;                                                          // 33
    };

    #define VGUI_SYSTEM_INTERFACE_VERSION "VGUI_System009"
}
