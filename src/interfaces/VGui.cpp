#include "vgui/CVGui.h"
#include "vgui/IPanel.h"
#include "vgui/IInput.h"
#include "vgui/IFileSystem.h"
#include "vgui/ILocalize.h"
#include "vgui/IKeyValues.h"
#include <cstring>

namespace vgui
{
    extern void Log(const char* format, ...);
    
    CVGui* g_pVGui = nullptr;

    CVGui::CVGui()
        : m_pFileSystem(nullptr)
        , m_pPanel(nullptr)
        , m_pLocalize(nullptr)
        , m_pKeyValues(nullptr)
        , m_pInput(nullptr)
    {
        Log("CVGui::CVGui()\n");
    }

    CVGui::~CVGui()
    {
        Log("CVGui::~CVGui()\n");
    }

    void CVGui::Init(CreateInterfaceFn* pFactoryList, int nFactoryCount)
    {
        Log("CVGui::Init() called with %d factories\n", nFactoryCount);
        
        if (!pFactoryList || nFactoryCount <= 0)
        {
            Log("CVGui::Init() - invalid factory list\n");
            return;
        }

        for (int i = 0; i < nFactoryCount; i++)
        {
            if (!pFactoryList[i])
            {
                Log("CVGui::Init() - factory[%d] is null\n", i);
                continue;
            }
            
            Log("CVGui::Init() - checking factory[%d] at %p\n", i, pFactoryList[i]);

            void* pInterface;

            pInterface = pFactoryList[i]("VGUI_Input004", nullptr);
            if (pInterface)
            {
                Log("CVGui::Init() - got VGUI_Input004\n");
                g_pInput = static_cast<IInput*>(pInterface);
                m_pInput = g_pInput;
            }

            pInterface = pFactoryList[i]("VFileSystem009", nullptr);
            if (pInterface)
            {
                Log("CVGui::Init() - got VFileSystem009\n");
                g_pFileSystem = static_cast<IFileSystem*>(pInterface);
                m_pFileSystem = g_pFileSystem;
            }

            pInterface = pFactoryList[i]("KeyValues003", nullptr);
            if (pInterface)
            {
                Log("CVGui::Init() - got KeyValues003\n");
                g_pKeyValues = static_cast<IKeyValues*>(pInterface);
                m_pKeyValues = g_pKeyValues;
            }

            pInterface = pFactoryList[i]("VGUI_Localize003", nullptr);
            if (pInterface)
            {
                Log("CVGui::Init() - got VGUI_Localize003\n");
                g_pLocalize = static_cast<ILocalize*>(pInterface);
                m_pLocalize = g_pLocalize;
            }

            pInterface = pFactoryList[i]("VGUI_Panel007", nullptr);
            if (pInterface)
            {
                Log("CVGui::Init() - got VGUI_Panel007\n");
                g_pPanel = static_cast<IPanel*>(pInterface);
                m_pPanel = g_pPanel;
            }
        }
        
        Log("CVGui::Init() completed\n");
    }

    void CVGui::Shutdown()
    {
        Log("CVGui::Shutdown()\n");
        
        g_pFileSystem = nullptr;
        g_pPanel = nullptr;
        g_pLocalize = nullptr;
        g_pKeyValues = nullptr;
        g_pInput = nullptr;

        m_pFileSystem = nullptr;
        m_pPanel = nullptr;
        m_pLocalize = nullptr;
        m_pKeyValues = nullptr;
        m_pInput = nullptr;
    }
}
