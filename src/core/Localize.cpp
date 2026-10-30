#include "vgui/ILocalize.h"
#include "vgui/Trace.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <map>
#include <string>

namespace vgui
{
    // Localize003: a real string table. The file parser understands the Valve text
    // format ("lang" { "Tokens" { "name" "value" } }), UTF-8 or UTF-16 (BOM).
    class CLocalize : public ILocalize
    {
    public:
        CLocalize() { Log("CLocalize::CLocalize()\n"); }

        bool AddFile(void* fileSystem, const char* fileName) override
        {
            VGUI_TRACE("ILocalize::AddFile(fs=%p, '%s')\n", fileSystem, fileName ? fileName : "(null)");
            if (!fileName) return false;
            // The engine's IFileSystem vtable is not reproduced, so read from disk directly.
            const char* prefixes[] = { "", "valve/", "valve_hd/", "valve_addon/" };
            for (const char* prefix : prefixes)
            {
                std::string path = std::string(prefix) + fileName;
                if (LoadFile(path.c_str(), fileName)) return true;
            }
            return false;
        }

        void RemoveAll() override { m_Entries.clear(); m_Index.clear(); m_Files.clear(); }

        const wchar_t* Find(const char* tokenName) override
        {
            StringIndex_t i = FindIndex(tokenName);
            return i == INVALID_LOCALIZE_STRING_INDEX ? nullptr : m_Entries[i].value.c_str();
        }

        int ConvertANSIToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes) override
        {
            if (!unicode || unicodeBufferSizeInBytes < (int)sizeof(wchar_t)) return 0;
            unicode[0] = 0;
            if (!ansi) return 1;
            int maxChars = unicodeBufferSizeInBytes / (int)sizeof(wchar_t);
            int n = MultiByteToWideChar(CP_ACP, 0, ansi, -1, unicode, maxChars);
            if (n <= 0) { unicode[maxChars - 1] = 0; return maxChars; }
            return n;
        }

        int ConvertUnicodeToANSI(const wchar_t* unicode, char* ansi, int ansiBufferSize) override
        {
            if (!ansi || ansiBufferSize <= 0) return 0;
            ansi[0] = 0;
            if (!unicode) return 1;
            int n = WideCharToMultiByte(CP_ACP, 0, unicode, -1, ansi, ansiBufferSize, nullptr, nullptr);
            if (n <= 0) { ansi[ansiBufferSize - 1] = 0; return ansiBufferSize; }
            return n;
        }

        StringIndex_t FindIndex(const char* tokenName) override
        {
            if (!tokenName) return INVALID_LOCALIZE_STRING_INDEX;
            if (*tokenName == '#') tokenName++;
            auto it = m_Index.find(Lower(tokenName));
            return it == m_Index.end() ? INVALID_LOCALIZE_STRING_INDEX : it->second;
        }

        // Variable substitution (%s1 ...) is not implemented yet: the format is copied as is.
        void ConstructStringFmt(wchar_t* out, int outBytes, const wchar_t* format, void* variables) override
        {
            CopyOut(out, outBytes, format);
        }
        void ConstructStringIdx(wchar_t* out, int outBytes, StringIndex_t index, void* variables) override
        {
            CopyOut(out, outBytes, GetValueByIndex(index));
        }

        int Slot9() override { VGUI_TRACE("ILocalize::Slot9\n"); return 0; }

        const char* GetNameByIndex(StringIndex_t index) override
        {
            return index < m_Entries.size() ? m_Entries[index].name.c_str() : "";
        }
        const wchar_t* GetValueByIndex(StringIndex_t index) override
        {
            return index < m_Entries.size() ? m_Entries[index].value.c_str() : L"";
        }
        StringIndex_t GetFirstStringIndex() override { return m_Entries.empty() ? INVALID_LOCALIZE_STRING_INDEX : 0; }
        StringIndex_t GetNextStringIndex(StringIndex_t index) override
        {
            return (index + 1 < m_Entries.size()) ? index + 1 : INVALID_LOCALIZE_STRING_INDEX;
        }

        void AddString(const char* tokenName, const wchar_t* unicodeString, const char* fileName) override
        {
            if (!tokenName) return;
            if (*tokenName == '#') tokenName++;
            std::string lower = Lower(tokenName);
            auto it = m_Index.find(lower);
            if (it != m_Index.end())
            {
                m_Entries[it->second].value = unicodeString ? unicodeString : L"";
                return;
            }
            Entry e;
            e.name = tokenName;
            e.value = unicodeString ? unicodeString : L"";
            e.file = fileName ? fileName : "";
            m_Entries.push_back(e);
            m_Index[lower] = (StringIndex_t)(m_Entries.size() - 1);
        }
        void SetValueByIndex(StringIndex_t index, const wchar_t* newValue) override
        {
            if (index < m_Entries.size()) m_Entries[index].value = newValue ? newValue : L"";
        }
        bool SaveToFile(void* fileSystem, const char* fileName) override { return false; }
        int GetLocalizationFileCount() override { return (int)m_Files.size(); }
        const char* GetLocalizationFileName(int index) override
        {
            return (index >= 0 && (size_t)index < m_Files.size()) ? m_Files[(size_t)index].c_str() : "";
        }
        const char* GetFileNameByIndex(StringIndex_t index) override
        {
            return index < m_Entries.size() ? m_Entries[index].file.c_str() : "";
        }
        int Slot20(int a1) override { VGUI_TRACE("ILocalize::Slot20(%d)\n", a1); return 0; }

    private:
        struct Entry { std::string name; std::wstring value; std::string file; };

        static std::string Lower(const char* s)
        {
            std::string r(s);
            for (char& c : r) if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
            return r;
        }
        static void CopyOut(wchar_t* out, int outBytes, const wchar_t* text)
        {
            if (!out || outBytes < (int)sizeof(wchar_t)) return;
            int maxChars = outBytes / (int)sizeof(wchar_t);
            int i = 0;
            if (text) for (; text[i] && i < maxChars - 1; i++) out[i] = text[i];
            out[i] = 0;
        }

        bool LoadFile(const char* path, const char* fileName)
        {
            FILE* f = fopen(path, "rb");
            if (!f) return false;
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            fseek(f, 0, SEEK_SET);
            if (size <= 0) { fclose(f); return false; }
            char* raw = (char*)malloc((size_t)size + 2);
            if (!raw) { fclose(f); return false; }
            size_t got = fread(raw, 1, (size_t)size, f);
            fclose(f);
            raw[got] = 0; raw[got + 1] = 0;

            // UTF-16LE with BOM -> UTF-8
            char* buffer = raw;
            if (got >= 2 && (unsigned char)raw[0] == 0xFF && (unsigned char)raw[1] == 0xFE)
            {
                const wchar_t* w = (const wchar_t*)(raw + 2);
                int wlen = (int)((got - 2) / 2);
                int n = WideCharToMultiByte(CP_UTF8, 0, w, wlen, nullptr, 0, nullptr, nullptr);
                buffer = (char*)malloc((size_t)n + 1);
                if (!buffer) { free(raw); return false; }
                WideCharToMultiByte(CP_UTF8, 0, w, wlen, buffer, n, nullptr, nullptr);
                buffer[n] = 0;
                free(raw);
                raw = nullptr;
            }
            else if (got >= 3 && (unsigned char)raw[0] == 0xEF && (unsigned char)raw[1] == 0xBB && (unsigned char)raw[2] == 0xBF)
            {
                buffer = raw + 3;
            }

            bool inTokens = false;
            char* p = buffer;
            char key[512] = {0};
            char value[4096] = {0};
            int state = 0; // 0=looking for key, 1=got key, 2=got value
            
            while (*p)
            {
                // Skip whitespace
                while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
                if (!*p) break;
                
                if (*p == '{')
                {
                    // Check if previous key was "Tokens"
                    if (_stricmp(key, "Tokens") == 0)
                    {
                        inTokens = true;
                    }
                    p++;
                    continue;
                }
                
                if (*p == '}')
                {
                    if (inTokens)
                    {
                        inTokens = false;
                    }
                    p++;
                    continue;
                }
                
                if (*p == '"')
                {
                    p++; // skip opening quote
                    char* start = p;
                    while (*p && *p != '"')
                    {
                        if (*p == '\\' && *(p+1))
                        {
                            p++; // skip escape
                        }
                        p++;
                    }
                    
                    size_t len = p - start;
                    
                    if (state == 0)
                    {
                        // This is a key
                        if (len >= sizeof(key)) len = sizeof(key) - 1;
                        memcpy(key, start, len);
                        key[len] = 0;
                        state = 1;
                    }
                    else if (state == 1)
                    {
                        // This is a value
                        if (len >= sizeof(value)) len = sizeof(value) - 1;
                        memcpy(value, start, len);
                        value[len] = 0;
                        
                        if (inTokens)
                        {
                            // Convert to wchar_t and store
                            wchar_t wvalue[4096];
                            MultiByteToWideChar(CP_UTF8, 0, value, -1, wvalue, sizeof(wvalue)/sizeof(wchar_t));
                            AddString(key, wvalue, fileName);
                        }
                        state = 0;
                    }
                    
                    if (*p == '"') p++;
                    continue;
                }
                
                p++;
            }
            

            if (raw) free(raw); else free(buffer);
            m_Files.push_back(fileName);
            Log("ILocalize: loaded '%s' (%u strings total)\n", path, (unsigned)m_Entries.size());
            return true;
        }

        std::deque<Entry> m_Entries;                       // deque: references stay valid on push_back
        std::map<std::string, StringIndex_t> m_Index;
        std::deque<std::string> m_Files;
    };

    // VGUI_Localize002: older interface, 18 slots; only the verified argument counts are kept.
    class ILocalize002
    {
    public:
        virtual ~ILocalize002() {}
        virtual int S1(int a1, int a2) = 0;
        virtual int S2() = 0;
        virtual int S3(int a1) = 0;
        virtual int S4(int a1, int a2, int a3) = 0;
        virtual int S5(int a1, int a2, int a3) = 0;
        virtual int S6(int a1) = 0;
        virtual int S7() = 0;
        virtual int S8(int a1) = 0;
        virtual int S9(int a1) = 0;
        virtual int S10() = 0;
        virtual int S11(int a1) = 0;
        virtual int S12() = 0;
        virtual int S13(int a1, int a2) = 0;
        virtual int S14(int a1, int a2) = 0;
        virtual int S15() = 0;
        virtual int S16(int a1) = 0;
        virtual int S17(int a1) = 0;
    };

    class CLocalize002 : public ILocalize002
    {
    public:
        int S1(int a1, int a2) override { VGUI_TRACE("ILocalize002::S1\n"); return 0; }
        int S2() override { VGUI_TRACE("ILocalize002::S2\n"); return 0; }
        int S3(int a1) override { VGUI_TRACE("ILocalize002::S3\n"); return 0; }
        int S4(int a1, int a2, int a3) override { VGUI_TRACE("ILocalize002::S4\n"); return 0; }
        int S5(int a1, int a2, int a3) override { VGUI_TRACE("ILocalize002::S5\n"); return 0; }
        int S6(int a1) override { VGUI_TRACE("ILocalize002::S6\n"); return 0; }
        int S7() override { VGUI_TRACE("ILocalize002::S7\n"); return 0; }
        int S8(int a1) override { VGUI_TRACE("ILocalize002::S8\n"); return 0; }
        int S9(int a1) override { VGUI_TRACE("ILocalize002::S9\n"); return 0; }
        int S10() override { VGUI_TRACE("ILocalize002::S10\n"); return 0; }
        int S11(int a1) override { VGUI_TRACE("ILocalize002::S11\n"); return 0; }
        int S12() override { VGUI_TRACE("ILocalize002::S12\n"); return 0; }
        int S13(int a1, int a2) override { VGUI_TRACE("ILocalize002::S13\n"); return 0; }
        int S14(int a1, int a2) override { VGUI_TRACE("ILocalize002::S14\n"); return 0; }
        int S15() override { VGUI_TRACE("ILocalize002::S15\n"); return 0; }
        int S16(int a1) override { VGUI_TRACE("ILocalize002::S16\n"); return 0; }
        int S17(int a1) override { VGUI_TRACE("ILocalize002::S17\n"); return 0; }
    };

    static CLocalize g_LocalizeInstance;
    static CLocalize002 g_Localize002Instance;

    void* CreateLocalizeReal()
    {
        Log("CreateLocalizeReal() returning %p\n", &g_LocalizeInstance);
        return &g_LocalizeInstance;
    }

    void* CreateLocalize002Real()
    {
        Log("CreateLocalize002Real() returning %p\n", &g_Localize002Instance);
        return &g_Localize002Instance;
    }
}
