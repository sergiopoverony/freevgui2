#include "vgui/ISchemeManager.h"
#include "vgui/Trace.h"
#include <map>
#include <string>

namespace vgui
{
    // Logging stub with sane values. GetIScheme() returns null for now: the IScheme
    // object has not been reconstructed yet (its vtable was not found among the
    // RTTI vtables of the original); a crash here would show up in the log.
    class CSchemeManager : public ISchemeManager
    {
    public:
        CSchemeManager() : m_nNext(2) { m_Tags[""] = 1; }

        HScheme LoadSchemeFromFile(const char* fileName, const char* tag) override
        {
            VGUI_TRACE("ISchemeManager::LoadSchemeFromFile('%s','%s')\n", fileName ? fileName : "(null)", tag ? tag : "(null)");
            std::string t = tag ? tag : "";
            auto it = m_Tags.find(t);
            if (it != m_Tags.end()) return it->second;
            HScheme h = m_nNext++;
            m_Tags[t] = h;
            return h;
        }
        void ReloadSchemes() override { VGUI_TRACE("ISchemeManager::ReloadSchemes\n"); }
        HScheme GetDefaultScheme() override { return 1; }
        HScheme GetScheme(const char* tag) override
        {
            VGUI_TRACE("ISchemeManager::GetScheme('%s')\n", tag ? tag : "(null)");
            auto it = m_Tags.find(tag ? tag : "");
            return it != m_Tags.end() ? it->second : 1;
        }
        void* GetImage(const char* imageName, bool hardwareFiltered) override
        {
            VGUI_TRACE("ISchemeManager::GetImage('%s')\n", imageName ? imageName : "(null)");
            return nullptr;
        }
        HTexture GetImageID(const char* imageName, bool hardwareFiltered) override
        {
            VGUI_TRACE("ISchemeManager::GetImageID('%s')\n", imageName ? imageName : "(null)");
            return 0;
        }
        void* GetIScheme(HScheme scheme) override
        {
            VGUI_TRACE("ISchemeManager::GetIScheme(%lu) -> NULL (IScheme not implemented)\n", scheme);
            return nullptr;
        }
        void Shutdown(bool full) override { VGUI_TRACE("ISchemeManager::Shutdown(%d)\n", (int)full); }
        int GetProportionalScaledValue(int normalizedValue) override { return normalizedValue * ScreenTall() / 480; }
        int GetProportionalNormalizedValue(int scaledValue) override { return scaledValue * 480 / ScreenTall(); }
        int Slot11() override { VGUI_TRACE("ISchemeManager::Slot11\n"); return 0; }
        int Slot12(int a1) override { VGUI_TRACE("ISchemeManager::Slot12(%d)\n", a1); return 0; }
        int Slot13(int a1) override { VGUI_TRACE("ISchemeManager::Slot13(%d)\n", a1); return 0; }

    private:
        static int ScreenTall()
        {
            int t = GetSystemMetrics(SM_CYSCREEN);
            return t > 0 ? t : 480;
        }

        std::map<std::string, HScheme> m_Tags;
        HScheme m_nNext;
    };

    static CSchemeManager g_SchemeManager;

    void* CreateSchemeManager()
    {
        Log("CreateSchemeManager() returning %p\n", &g_SchemeManager);
        return &g_SchemeManager;
    }
}
