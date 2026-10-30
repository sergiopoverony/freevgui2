#include "vgui/IKeyValues.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <map>
#include <mutex>
#include <string>

namespace vgui
{
    extern void Log(const char* format, ...);

    // Implementation of IKeyValuesSystem ("KeyValues003").
    // Every method logs its first calls (arguments included) so that the real
    // vtable order can be confirmed from vgui2_debug.log.
    class CKeyValuesSystem : public IKeyValues
    {
    public:
        CKeyValuesSystem() : m_nSizeofKeyValues(0)
        {
            memset(m_nCalls, 0, sizeof(m_nCalls));
        }

        void RegisterSizeofKeyValues(int size) override
        {
            m_nSizeofKeyValues = size;
            Log("IKeyValues::RegisterSizeofKeyValues(%d)\n", size);
        }

        void* AllocKeyValuesMemory(int size) override
        {
            // calloc: KeyValues constructor initializes the object anyway, zeroing is just safer.
            void* p = calloc(1, size > 0 ? (size_t)size : 1);
            if (Trace(0)) Log("IKeyValues::AllocKeyValuesMemory(%d) -> %p\n", size, p);
            return p;
        }

        void FreeKeyValuesMemory(void* pMem) override
        {
            if (Trace(1)) Log("IKeyValues::FreeKeyValuesMemory(%p)\n", pMem);
            free(pMem);
        }

        HKeySymbol GetSymbolForString(const char* name) override
        {
            if (!name) name = "";
            std::string key = Lower(name);
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto it = m_SymbolIndex.find(key);
            HKeySymbol sym;
            if (it != m_SymbolIndex.end())
            {
                sym = it->second;
            }
            else
            {
                m_Strings.push_back(name);                 // keeps original case
                sym = (HKeySymbol)m_Strings.size();        // 1-based, 0 is never a valid symbol
                m_SymbolIndex[key] = sym;
            }
            if (Trace(2)) Log("IKeyValues::GetSymbolForString('%s') -> %d\n", name, sym);
            return sym;
        }

        const char* GetStringForSymbol(HKeySymbol symbol) override
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            const char* s = "";
            if (symbol >= 1 && (size_t)symbol <= m_Strings.size())
                s = m_Strings[(size_t)symbol - 1].c_str();
            if (Trace(3)) Log("IKeyValues::GetStringForSymbol(%d) -> '%s'\n", symbol, s);
            return s;
        }

        void GetLocalizedFromANSI(const char* ansi, wchar_t* outBuf, int unicodeBufferSizeInBytes) override
        {
            if (Trace(4)) Log("IKeyValues::GetLocalizedFromANSI('%s', %p, %d)\n", ansi ? ansi : "(null)", outBuf, unicodeBufferSizeInBytes);
            if (!outBuf || unicodeBufferSizeInBytes < (int)sizeof(wchar_t)) return;
            int maxChars = unicodeBufferSizeInBytes / (int)sizeof(wchar_t);
            outBuf[0] = 0;
            if (!ansi) return;
            int n = MultiByteToWideChar(CP_ACP, 0, ansi, -1, outBuf, maxChars);
            if (n <= 0) outBuf[maxChars - 1] = 0;
        }

        void GetANSIFromLocalized(const wchar_t* wchar, char* outBuf, int ansiBufferSizeInBytes) override
        {
            if (Trace(5)) Log("IKeyValues::GetANSIFromLocalized(%p, %p, %d)\n", wchar, outBuf, ansiBufferSizeInBytes);
            if (!outBuf || ansiBufferSizeInBytes <= 0) return;
            outBuf[0] = 0;
            if (!wchar) return;
            int n = WideCharToMultiByte(CP_ACP, 0, wchar, -1, outBuf, ansiBufferSizeInBytes, nullptr, nullptr);
            if (n <= 0) outBuf[ansiBufferSizeInBytes - 1] = 0;
        }

        void AddKeyValuesToMemoryLeakList(void* pMem, HKeySymbol name) override {}
        void RemoveKeyValuesFromMemoryLeakList(void* pMem) override {}

    private:
        static std::string Lower(const char* s)
        {
            std::string r(s);
            for (char& c : r) if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
            return r;
        }

        // Log only the first 64 calls of each method to keep the log readable.
        bool Trace(int idx) { return m_nCalls[idx]++ < 64; }

        int m_nSizeofKeyValues;
        int m_nCalls[8];
        std::mutex m_Mutex;
        std::deque<std::string> m_Strings;            // deque: references stay valid on push_back
        std::map<std::string, HKeySymbol> m_SymbolIndex;
    };

    static CKeyValuesSystem g_KeyValuesInstance;

    void* CreateKeyValuesReal()
    {
        return &g_KeyValuesInstance;
    }
}
