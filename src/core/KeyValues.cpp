#include "vgui/IKeyValues.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace vgui
{
    class CKeyValuesNode
    {
    public:
        std::string m_Name;
        std::string m_Value;
        std::map<std::string, CKeyValuesNode*> m_Children;
        std::vector<CKeyValuesNode*> m_OrderedChildren;
        
        CKeyValuesNode(const char* name) : m_Name(name ? name : "") {}
        
        ~CKeyValuesNode()
        {
            for (auto& pair : m_Children)
            {
                delete pair.second;
            }
        }
        
        void SetString(const char* key, const char* value)
        {
            auto it = m_Children.find(key);
            if (it != m_Children.end())
            {
                it->second->m_Value = value ? value : "";
            }
            else
            {
                CKeyValuesNode* pNode = new CKeyValuesNode(key);
                pNode->m_Value = value ? value : "";
                m_Children[key] = pNode;
                m_OrderedChildren.push_back(pNode);
            }
        }
        
        const char* GetString(const char* key, const char* defaultValue = "")
        {
            auto it = m_Children.find(key);
            if (it != m_Children.end())
            {
                return it->second->m_Value.c_str();
            }
            return defaultValue;
        }
        
        int GetInt(const char* key, int defaultValue = 0)
        {
            auto it = m_Children.find(key);
            if (it != m_Children.end())
            {
                return atoi(it->second->m_Value.c_str());
            }
            return defaultValue;
        }
        
        float GetFloat(const char* key, float defaultValue = 0.0f)
        {
            auto it = m_Children.find(key);
            if (it != m_Children.end())
            {
                return (float)atof(it->second->m_Value.c_str());
            }
            return defaultValue;
        }
        
        CKeyValuesNode* GetSubKey(const char* name)
        {
            auto it = m_Children.find(name);
            if (it != m_Children.end())
            {
                return it->second;
            }
            return nullptr;
        }
        
        CKeyValuesNode* CreateNewKey(const char* name)
        {
            CKeyValuesNode* pNode = new CKeyValuesNode(name);
            m_Children[name] = pNode;
            m_OrderedChildren.push_back(pNode);
            return pNode;
        }
        
        CKeyValuesNode* GetFirstSubKey()
        {
            if (m_OrderedChildren.empty()) return nullptr;
            return m_OrderedChildren[0];
        }
        
        CKeyValuesNode* GetNextKey()
        {
            // This requires parent context, simplified version
            return nullptr;
        }
    };
    
    class CKeyValues : public IKeyValues
    {
    public:
        void* CreateKeyValues(const char* name) override
        {
            return new CKeyValuesNode(name);
        }
        
        void DeleteKeyValues(void* kv) override
        {
            if (kv)
            {
                delete static_cast<CKeyValuesNode*>(kv);
            }
        }
        
        bool LoadFromFile(void* kv, const char* fileName)
        {
            if (!kv || !fileName) return false;
            
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
            
            CKeyValuesNode* pNode = static_cast<CKeyValuesNode*>(kv);
            bool result = ParseBuffer(pNode, buffer);
            
            free(buffer);
            return result;
        }
        
        bool SaveToFile(void* kv, const char* fileName)
        {
            if (!kv || !fileName) return false;
            
            FILE* f = fopen(fileName, "wb");
            if (!f) return false;
            
            CKeyValuesNode* pNode = static_cast<CKeyValuesNode*>(kv);
            WriteNode(f, pNode, 0);
            
            fclose(f);
            return true;
        }
        
    private:
        bool ParseBuffer(CKeyValuesNode* pNode, const char* buffer)
        {
            const char* p = buffer;
            
            while (*p)
            {
                // Skip whitespace
                while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
                if (!*p) break;
                
                if (*p == '{')
                {
                    p++;
                    continue;
                }
                
                if (*p == '}')
                {
                    p++;
                    return true;
                }
                
                if (*p == '"')
                {
                    p++; // skip opening quote
                    const char* start = p;
                    while (*p && *p != '"')
                    {
                        if (*p == '\\' && *(p+1)) p++;
                        p++;
                    }
                    
                    size_t len = p - start;
                    char key[512];
                    if (len >= sizeof(key)) len = sizeof(key) - 1;
                    memcpy(key, start, len);
                    key[len] = 0;
                    
                    if (*p == '"') p++;
                    
                    // Skip whitespace
                    while (*p == ' ' || *p == '\t') p++;
                    
                    if (*p == '"')
                    {
                        p++; // skip opening quote
                        start = p;
                        while (*p && *p != '"')
                        {
                            if (*p == '\\' && *(p+1)) p++;
                            p++;
                        }
                        
                        len = p - start;
                        char value[4096];
                        if (len >= sizeof(value)) len = sizeof(value) - 1;
                        memcpy(value, start, len);
                        value[len] = 0;
                        
                        if (*p == '"') p++;
                        
                        pNode->SetString(key, value);
                    }
                    else if (*p == '{')
                    {
                        CKeyValuesNode* pSubKey = pNode->CreateNewKey(key);
                        p++; // skip '{'
                        ParseBuffer(pSubKey, p);
                        
                        // Find closing brace
                        while (*p && *p != '}') p++;
                        if (*p == '}') p++;
                    }
                }
                else
                {
                    p++;
                }
            }
            
            return true;
        }
        
        void WriteNode(FILE* f, CKeyValuesNode* pNode, int indent)
        {
            for (int i = 0; i < indent; i++) fprintf(f, "\t");
            fprintf(f, "\"%s\"\n", pNode->m_Name.c_str());
            
            for (int i = 0; i < indent; i++) fprintf(f, "\t");
            fprintf(f, "{\n");
            
            for (auto* pChild : pNode->m_OrderedChildren)
            {
                if (pChild->m_OrderedChildren.empty())
                {
                    // Key-value pair
                    for (int i = 0; i <= indent; i++) fprintf(f, "\t");
                    fprintf(f, "\"%s\"\t\t\"%s\"\n", pChild->m_Name.c_str(), pChild->m_Value.c_str());
                }
                else
                {
                    // Sub-section
                    WriteNode(f, pChild, indent + 1);
                }
            }
            
            for (int i = 0; i < indent; i++) fprintf(f, "\t");
            fprintf(f, "}\n");
        }
    };
    
    static CKeyValues g_KeyValuesInstance;
    
    void* CreateKeyValuesReal()
    {
        return &g_KeyValuesInstance;
    }
}
