#pragma once
#include "VGui.h"

namespace vgui
{
    class CVGui
    {
    public:
        CVGui();
        ~CVGui();
        
        void Init(CreateInterfaceFn* pFactoryList, int nFactoryCount);
        void Shutdown();
        
    private:
        IFileSystem* m_pFileSystem;
        IPanel* m_pPanel;
        ILocalize* m_pLocalize;
        IKeyValues* m_pKeyValues;
        IInput* m_pInput;
    };
    
    extern CVGui* g_pVGui;
    extern IFileSystem* g_pFileSystem;
    extern IPanel* g_pPanel;
    extern ILocalize* g_pLocalize;
    extern IKeyValues* g_pKeyValues;
    extern IInput* g_pInput;
}
