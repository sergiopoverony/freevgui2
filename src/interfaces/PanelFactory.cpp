#include "vgui/IPanelFactory.h"
#include "vgui/VPanel.h"
#include <map>
#include <string>

namespace vgui
{
    extern HPanel AllocatePanel(const char* className, HPanel parentId);
    extern VPanel* GetVPanel(HPanel panel);
    
    class CPanelFactory : public IPanelFactory
    {
    public:
        CPanelFactory(const char* className) : m_ClassName(className) {}
        virtual ~CPanelFactory() {}
        
        HPanel CreatePanel(const char* className, HPanel parentId) override
        {
            return AllocatePanel(className, parentId);
        }
        
        const char* GetClassName() override
        {
            return m_ClassName.c_str();
        }
        
    private:
        std::string m_ClassName;
    };
    
    class CPanelFactoryManager : public IPanelFactoryManager
    {
    public:
        CPanelFactoryManager()
        {
            // Register default factories
            RegisterFactory(new CPanelFactory("Panel"));
            RegisterFactory(new CPanelFactory("Button"));
            RegisterFactory(new CPanelFactory("Label"));
        }
        
        ~CPanelFactoryManager()
        {
            for (auto& pair : m_Factories)
            {
                delete pair.second;
            }
        }
        
        void RegisterFactory(IPanelFactory* factory) override
        {
            if (!factory) return;
            const char* className = factory->GetClassName();
            if (className)
            {
                m_Factories[className] = factory;
            }
        }
        
        HPanel CreatePanel(const char* className, HPanel parentId) override
        {
            if (!className) return INVALID_PANEL;
            
            auto it = m_Factories.find(className);
            if (it != m_Factories.end())
            {
                return it->second->CreatePanel(className, parentId);
            }
            
            // Fallback to generic panel
            return AllocatePanel(className, parentId);
        }
        
        void UnregisterFactory(const char* className) override
        {
            if (!className) return;
            auto it = m_Factories.find(className);
            if (it != m_Factories.end())
            {
                delete it->second;
                m_Factories.erase(it);
            }
        }
        
    private:
        std::map<std::string, IPanelFactory*> m_Factories;
    };
    
    static CPanelFactoryManager g_PanelFactoryManager;
    
    IPanelFactoryManager* GetPanelFactoryManager()
    {
        return &g_PanelFactoryManager;
    }
}
