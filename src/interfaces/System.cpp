#include "vgui/ISystem.h"
#include <windows.h>
#include <cstdio>
#include <cstring>

namespace vgui
{
    extern void Log(const char* format, ...);
    
    class CSystem : public ISystem
    {
    public:
        CSystem() { Log("CSystem::CSystem()\n"); }
        ~CSystem() {}
        
        void Shutdown() override { Log("ISystem::Shutdown()\n"); }
        void RunFrame() override { Log("ISystem::RunFrame()\n"); }
        
        bool GetDesktopResolution(int& wide, int& tall) override
        {
            Log("ISystem::GetDesktopResolution()\n");
            HDC hdc = GetDC(nullptr);
            wide = GetDeviceCaps(hdc, HORZRES);
            tall = GetDeviceCaps(hdc, VERTRES);
            ReleaseDC(nullptr, hdc);
            Log("  -> %dx%d\n", wide, tall);
            return true;
        }
        
        bool GetCursorPosition(int& x, int& y) override
        {
            Log("ISystem::GetCursorPosition()\n");
            POINT pt;
            if (GetCursorPos(&pt))
            {
                x = pt.x;
                y = pt.y;
                return true;
            }
            return false;
        }
        
        void SetCursorPosition(int x, int y) override
        {
            Log("ISystem::SetCursorPosition(%d, %d)\n", x, y);
            ::SetCursorPos(x, y);
        }
        
        void Sleep(unsigned int milliseconds) override
        {
            Log("ISystem::Sleep(%u)\n", milliseconds);
            ::Sleep(milliseconds);
        }
        
        void* OpenFile(const char* fileName, const char* options) override
        {
            Log("ISystem::OpenFile('%s', '%s')\n", fileName ? fileName : "null", options ? options : "null");
            return fopen(fileName, options);
        }
        
        void CloseFile(void* file) override
        {
            Log("ISystem::CloseFile(%p)\n", file);
            if (file) fclose((FILE*)file);
        }
        
        unsigned long GetFileSize(void* file) override
        {
            Log("ISystem::GetFileSize(%p)\n", file);
            if (!file) return 0;
            FILE* f = (FILE*)file;
            long pos = ftell(f);
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            fseek(f, pos, SEEK_SET);
            return (unsigned long)size;
        }
        
        bool FileExists(const char* fileName) override
        {
            Log("ISystem::FileExists('%s')\n", fileName ? fileName : "null");
            if (!fileName) return false;
            FILE* f = fopen(fileName, "rb");
            if (f)
            {
                fclose(f);
                return true;
            }
            return false;
        }
        
        void SetClipboardText(const wchar_t* text, int textLen) override
        {
            Log("ISystem::SetClipboardText()\n");
            if (!text) return;
            if (OpenClipboard(nullptr))
            {
                EmptyClipboard();
                size_t len = (textLen > 0 ? textLen : (int)wcslen(text)) + 1;
                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(wchar_t));
                if (hMem)
                {
                    wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                    memcpy(pMem, text, len * sizeof(wchar_t));
                    GlobalUnlock(hMem);
                    SetClipboardData(CF_UNICODETEXT, hMem);
                }
                CloseClipboard();
            }
        }
        
        void GetClipboardText(int maxLen, wchar_t* buffer) override
        {
            Log("ISystem::GetClipboardText(%d)\n", maxLen);
            if (!buffer || maxLen <= 0) return;
            buffer[0] = 0;
            
            if (OpenClipboard(nullptr))
            {
                HANDLE hData = GetClipboardData(CF_UNICODETEXT);
                if (hData)
                {
                    wchar_t* pText = (wchar_t*)GlobalLock(hData);
                    if (pText)
                    {
                        wcsncpy(buffer, pText, maxLen - 1);
                        buffer[maxLen - 1] = 0;
                        GlobalUnlock(hData);
                    }
                }
                CloseClipboard();
            }
        }
        
        bool SetVideoMode(int wide, int tall, int bpp) override
        {
            Log("ISystem::SetVideoMode(%d, %d, %d)\n", wide, tall, bpp);
            return false;
        }
        
        void GetVideoMode(int& wide, int& tall, int& bpp) override
        {
            Log("ISystem::GetVideoMode()\n");
            wide = 800;
            tall = 600;
            bpp = 32;
        }
    };
    
    static CSystem g_SystemInstance;
    
    void* CreateSystemReal()
    {
        Log("CreateSystemReal() returning %p\n", &g_SystemInstance);
        return &g_SystemInstance;
    }
}
