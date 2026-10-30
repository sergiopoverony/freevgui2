#include "vgui/ILocalize.h"
#include <windows.h>
#include <map>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace vgui
{
    class CLocalize : public ILocalize
    {
    public:
        CLocalize() {}
        ~CLocalize() { RemoveAll(); }
        
        bool AddFile(const char* fileName) override
        {
            if (!fileName) return false;
            
            FILE* f = fopen(fileName, "rb");
            if (!f) return false;
            
            fseek(f, 0, SEEK_END);
            long size = ftell(f);
            fseek(f, 0, SEEK_SET);
            
            char* buffer = (char*)malloc(size + 1);
            if (!buffer)
            {
                fclose(f);
                return false;
            }
            
            fread(buffer, 1, size, f);
            buffer[size] = 0;
            fclose(f);
            
            // Parse Valve localization format:
            // "lang"
            // {
            //     "Language" "English"
            //     "Tokens"
            //     {
            //         "token_name" "Token Value"
            //     }
            // }
            
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
                            m_Strings[key] = _wcsdup(wvalue);
                        }
                        state = 0;
                    }
                    
                    if (*p == '"') p++;
                    continue;
                }
                
                p++;
            }
            
            free(buffer);
            return true;
        }
        
        void RemoveAll() override
        {
            for (auto& pair : m_Strings)
            {
                if (pair.second) free((void*)pair.second);
            }
            m_Strings.clear();
        }
        
        const wchar_t* Find(const char* tokenName) override
        {
            if (!tokenName) return nullptr;
            auto it = m_Strings.find(tokenName);
            if (it != m_Strings.end())
            {
                return it->second;
            }
            return nullptr;
        }
        
    private:
        std::map<std::string, const wchar_t*> m_Strings;
    };
    
    static CLocalize g_LocalizeInstance;
    
    void* CreateLocalizeReal()
    {
        return &g_LocalizeInstance;
    }
}
