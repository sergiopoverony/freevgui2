#pragma once
#include "VGui.h"

namespace vgui
{
    class ISystem
    {
    public:
        virtual ~ISystem() {}
        virtual void Shutdown() = 0;
        virtual void RunFrame() = 0;
        virtual bool GetDesktopResolution(int& wide, int& tall) = 0;
        virtual bool GetCursorPosition(int& x, int& y) = 0;
        virtual void SetCursorPosition(int x, int y) = 0;
        virtual void Sleep(unsigned int milliseconds) = 0;
        virtual void* OpenFile(const char* fileName, const char* options) = 0;
        virtual void CloseFile(void* file) = 0;
        virtual unsigned long GetFileSize(void* file) = 0;
        virtual bool FileExists(const char* fileName) = 0;
        virtual void SetClipboardText(const wchar_t* text, int textLen) = 0;
        virtual void GetClipboardText(int maxLen, wchar_t* buffer) = 0;
        virtual bool SetVideoMode(int wide, int tall, int bpp) = 0;
        virtual void GetVideoMode(int& wide, int& tall, int& bpp) = 0;
    };
    
    #define VGUI_SYSTEM_INTERFACE_VERSION "VGUI_System009"
}
