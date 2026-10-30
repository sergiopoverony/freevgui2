#include "vgui/ISchemeManager.h"
#include "vgui/IKeyValues.h"
#include <map>
#include <string>
#include <cstdlib>
#include <cstring>

namespace vgui
{
    class CSchemeManager : public ISchemeManager
    {
    public:
        struct Scheme
        {
            std::string m_Name;
            std::map<std::string, std::string> m_Colors;
            std::map<std::string, HFont> m_Fonts;
            bool m_bProportional;
        };
        
        CSchemeManager()
        {
            m_nNextSchemeID = 1;
        }
        
        ~CSchemeManager()
        {
            for (auto& pair : m_Schemes)
            {
                delete pair.second;
            }
        }
        
        HScheme GetScheme(const char* schemeName, bool proportional) override
        {
            if (!schemeName) return 0;
            
            for (auto& pair : m_Schemes)
            {
                if (pair.second->m_Name == schemeName && pair.second->m_bProportional == proportional)
                {
                    return pair.first;
                }
            }
            
            Scheme* pScheme = new Scheme();
            pScheme->m_Name = schemeName;
            pScheme->m_bProportional = proportional;
            
            HScheme id = m_nNextSchemeID++;
            m_Schemes[id] = pScheme;
            
            return id;
        }
        
        const char* GetColor(HScheme scheme, const char* colorName) override
        {
            auto it = m_Schemes.find(scheme);
            if (it == m_Schemes.end()) return nullptr;
            
            auto colorIt = it->second->m_Colors.find(colorName);
            if (colorIt == it->second->m_Colors.end()) return nullptr;
            
            return colorIt->second.c_str();
        }
        
        HFont GetFont(HScheme scheme, const char* fontName) override
        {
            auto it = m_Schemes.find(scheme);
            if (it == m_Schemes.end()) return INVALID_FONT;
            
            auto fontIt = it->second->m_Fonts.find(fontName);
            if (fontIt == it->second->m_Fonts.end()) return INVALID_FONT;
            
            return fontIt->second;
        }
        
        void ReloadSchemes() override
        {
            for (auto& pair : m_Schemes)
            {
                delete pair.second;
            }
            m_Schemes.clear();
            m_nNextSchemeID = 1;
        }
        
        void AddCustomFont(const char* fontName) override
        {
            // TODO: Implement custom font registration
        }
        
        void SetColor(HScheme scheme, const char* colorName, const char* colorValue)
        {
            auto it = m_Schemes.find(scheme);
            if (it == m_Schemes.end()) return;
            
            it->second->m_Colors[colorName] = colorValue;
        }
        
        void SetFont(HScheme scheme, const char* fontName, HFont font)
        {
            auto it = m_Schemes.find(scheme);
            if (it == m_Schemes.end()) return;
            
            it->second->m_Fonts[fontName] = font;
        }
        
    private:
        std::map<HScheme, Scheme*> m_Schemes;
        HScheme m_nNextSchemeID;
    };
    
    static CSchemeManager g_SchemeManager;
    
    void* CreateSchemeManager()
    {
        return &g_SchemeManager;
    }
}
