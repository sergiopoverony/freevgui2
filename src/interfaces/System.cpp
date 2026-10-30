#include "vgui/ISystem.h"
#include "vgui/Trace.h"
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

namespace vgui
{
    class CSystem : public ISystem
    {
    public:
        CSystem() : m_dLastFrame(0.0), m_dFrameTime(0.0)
        {
            QueryPerformanceFrequency(&m_Freq);
            QueryPerformanceCounter(&m_Start);
            Log("CSystem::CSystem()\n");
        }

        void Shutdown() override { Log("ISystem::Shutdown()\n"); }
        void RunFrame() override
        {
            double now = Now();
            m_dFrameTime = now - m_dLastFrame;
            m_dLastFrame = now;
        }
        void ShellExecute(const char* command, const char* file) override
        {
            VGUI_TRACE("ISystem::ShellExecute('%s','%s')\n", command ? command : "(null)", file ? file : "(null)");
        }
        double GetFrameTime() override { return m_dFrameTime; }
        double GetCurrentTime() override { return Now(); }
        long GetTimeMillis() override { return (long)(Now() * 1000.0); }

        int GetClipboardTextCount() override
        {
            int n = 0;
            if (OpenClipboard(nullptr))
            {
                HANDLE h = GetClipboardData(CF_UNICODETEXT);
                if (h)
                {
                    const wchar_t* p = (const wchar_t*)GlobalLock(h);
                    if (p) { n = (int)wcslen(p); GlobalUnlock(h); }
                }
                CloseClipboard();
            }
            return n;
        }
        void SetClipboardTextA(const char* text, int textLen) override
        {
            if (!text) return;
            int n = textLen > 0 ? textLen : (int)strlen(text);
            std::wstring w((size_t)n + 1, L'\0');
            int c = MultiByteToWideChar(CP_ACP, 0, text, n, &w[0], n);
            SetClipboardTextW(w.c_str(), c);
        }
        void SetClipboardTextW(const wchar_t* text, int textLen) override
        {
            if (!text) return;
            size_t n = textLen > 0 ? (size_t)textLen : wcslen(text);
            if (!OpenClipboard(nullptr)) return;
            EmptyClipboard();
            HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, (n + 1) * sizeof(wchar_t));
            if (h)
            {
                wchar_t* p = (wchar_t*)GlobalLock(h);
                memcpy(p, text, n * sizeof(wchar_t));
                p[n] = 0;
                GlobalUnlock(h);
                SetClipboardData(CF_UNICODETEXT, h);
            }
            CloseClipboard();
        }
        int GetClipboardTextA(int offset, char* buf, int bufLen) override
        {
            if (!buf || bufLen <= 0) return 0;
            buf[0] = 0;
            std::wstring w(bufLen, L'\0');
            int n = GetClipboardTextW(offset, &w[0], bufLen);
            if (n <= 0) return 0;
            int r = WideCharToMultiByte(CP_ACP, 0, w.c_str(), n, buf, bufLen - 1, nullptr, nullptr);
            buf[r > 0 ? r : 0] = 0;
            return r;
        }
        int GetClipboardTextW(int offset, wchar_t* buf, int bufLen) override
        {
            if (!buf || bufLen <= 0) return 0;
            buf[0] = 0;
            int copied = 0;
            if (OpenClipboard(nullptr))
            {
                HANDLE h = GetClipboardData(CF_UNICODETEXT);
                if (h)
                {
                    const wchar_t* p = (const wchar_t*)GlobalLock(h);
                    if (p)
                    {
                        int total = (int)wcslen(p);
                        if (offset < 0) offset = 0;
                        if (offset < total)
                        {
                            copied = total - offset;
                            if (copied > bufLen - 1) copied = bufLen - 1;
                            memcpy(buf, p + offset, copied * sizeof(wchar_t));
                            buf[copied] = 0;
                        }
                        GlobalUnlock(h);
                    }
                }
                CloseClipboard();
            }
            return copied;
        }

        // The registry is kept in memory only (not persisted, nothing written to the real registry).
        bool SetRegistryString(const char* key, const char* value) override
        {
            if (!key) return false;
            m_Registry[key] = value ? value : "";
            return true;
        }
        bool GetRegistryString(const char* key, char* value, int valueLen) override
        {
            if (!key || !value || valueLen <= 0) return false;
            auto it = m_Registry.find(key);
            if (it == m_Registry.end()) { value[0] = 0; return false; }
            strncpy(value, it->second.c_str(), (size_t)valueLen - 1);
            value[valueLen - 1] = 0;
            return true;
        }
        bool SetRegistryInteger(const char* key, int value) override
        {
            if (!key) return false;
            m_RegistryInt[key] = value;
            return true;
        }
        bool GetRegistryInteger(const char* key, int& value) override
        {
            if (!key) return false;
            auto it = m_RegistryInt.find(key);
            if (it == m_RegistryInt.end()) return false;
            value = it->second;
            return true;
        }
        void* GetUserConfigFileData(const char* dialogName, int dialogID) override
        {
            VGUI_TRACE("ISystem::GetUserConfigFileData('%s',%d)\n", dialogName ? dialogName : "(null)", dialogID);
            return nullptr;
        }
        void SetUserConfigFile(const char* fileName, const char* pathName) override
        {
            VGUI_TRACE("ISystem::SetUserConfigFile('%s','%s')\n", fileName ? fileName : "(null)", pathName ? pathName : "(null)");
        }
        void SaveUserConfigFile() override {}
        bool SetWatchForComputerUse(bool state) override { return true; }
        double GetTimeSinceLastUse() override { return 0.0; }

        int GetAvailableDrives(char* buf, int bufLen) override
        {
            if (!buf || bufLen <= 0) return 0;
            DWORD n = GetLogicalDriveStringsA((DWORD)bufLen - 1, buf);
            return (int)n;
        }
        bool CommandLineParamExists(const char* paramName) override
        {
            if (!paramName) return false;
            std::string cl = GetCommandLineA();
            return cl.find(paramName) != std::string::npos;
        }
        int Slot23(int a1, int a2) override { VGUI_TRACE("ISystem::Slot23(%d,%d)\n", a1, a2); return 0; }
        bool GetCurrentTimeAndDate(int* year, int* month, int* dayOfWeek, int* day, int* hour, int* minute, int* second) override
        {
            SYSTEMTIME t;
            GetLocalTime(&t);
            if (year) *year = t.wYear;
            if (month) *month = t.wMonth;
            if (dayOfWeek) *dayOfWeek = t.wDayOfWeek;
            if (day) *day = t.wDay;
            if (hour) *hour = t.wHour;
            if (minute) *minute = t.wMinute;
            if (second) *second = t.wSecond;
            return true;
        }
        double GetFreeDiskSpace(const char* path) override { return 1024.0 * 1024.0 * 1024.0; }
        bool CreateShortcut(const char*, const char*, const char*, const char*, const char*) override { return false; }
        bool GetShortcutTarget(const char*, char*, char*, int) override { return false; }
        bool ModifyShortcutTarget(const char*, const char*, const char*, const char*) override { return false; }
        bool GetCommandLineParamValue(const char* paramName, char* value, int valueBufferSize) override
        {
            if (value && valueBufferSize > 0) value[0] = 0;
            return false;
        }
        bool DeleteRegistryKey(const char* keyName) override { return true; }
        const char* GetDesktopFolderPath() override { return ""; }
        int Slot32(int a1) override { VGUI_TRACE("ISystem::Slot32(%d)\n", a1); return 0; }
        int Slot33(int a1) override { VGUI_TRACE("ISystem::Slot33(%d)\n", a1); return 0; }

    private:
        double Now() const
        {
            LARGE_INTEGER c;
            QueryPerformanceCounter(&c);
            return (double)(c.QuadPart - m_Start.QuadPart) / (double)m_Freq.QuadPart;
        }

        LARGE_INTEGER m_Freq, m_Start;
        double m_dLastFrame, m_dFrameTime;
        std::map<std::string, std::string> m_Registry;
        std::map<std::string, int> m_RegistryInt;
    };

    static CSystem g_SystemInstance;

    void* CreateSystemReal()
    {
        Log("CreateSystemReal() returning %p\n", &g_SystemInstance);
        return &g_SystemInstance;
    }
}
