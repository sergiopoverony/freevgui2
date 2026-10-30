#include "vgui/ISchemeManager.h"
#include "vgui/Trace.h"
#include "vgui/ISurface.h"
#include <windows.h>
#include <map>
#include <string>

namespace vgui
{
    // Logging stub with sane values. GetIScheme() returns null for now: the IScheme
    // object has not been reconstructed yet (its vtable was not found among the
    // RTTI vtables of the original); a crash here would show up in the log.
    // Placeholder IImage so that callers never dereference NULL. Layout follows the
    // Source SDK (Paint, SetPos, GetContentSize, GetSize, SetSize, SetColor, dtor, ...);
    // it draws nothing and reports a small fixed size.
    class CDummyImage
    {
    public:
        virtual void Paint() {}                                              // 0
        virtual void SetPos(int x, int y) {}                                 // 1
        virtual void GetContentSize(int& wide, int& tall) { wide = tall = 16; } // 2
        virtual void GetSize(int& wide, int& tall) { wide = m_w; tall = m_t; }  // 3
        virtual void SetSize(int wide, int tall) { m_w = wide; m_t = tall; }    // 4
        virtual void SetColor(Color col) {}                                  // 5
        virtual ~CDummyImage() {}                                            // 6
        virtual bool Evict() { return false; }                               // 7
        virtual int GetNumFrames() { return 0; }                             // 8
        virtual void SetFrame(int nFrame) {}                                 // 9
        virtual HTexture GetID() { return 0; }                               // 10
        virtual void SetRotation(int iRotation) {}                           // 11
    private:
        int m_w = 16, m_t = 16;
    };
    static CDummyImage g_DummyImage;

    // Minimal IScheme: no resources, default colors, no fonts. Enough to stop the engine
    // from dereferencing NULL; real scheme file parsing can be added later.
    class CScheme : public IScheme
    {
    public:
        const char* GetResourceString(const char* name) override
        {
            VGUI_TRACE("IScheme::GetResourceString('%s')\n", name ? name : "(null)");
            return "";
        }
        void* GetBorder(const char* name) override
        {
            VGUI_TRACE("IScheme::GetBorder('%s') -> NULL\n", name ? name : "(null)");
            return nullptr;
        }
        HFont GetFont(const char* name, bool proportional) override
        {
            VGUI_TRACE("IScheme::GetFont('%s', %d) -> 0\n", name ? name : "(null)", (int)proportional);
            return 0;
        }
        Color GetColor(const char* name, Color def) override
        {
            VGUI_TRACE("IScheme::GetColor('%s') -> default\n", name ? name : "(null)");
            return def;
        }
        HFont GetFontEx(const char* name, bool a, bool b) override
        {
            VGUI_TRACE("IScheme::GetFontEx('%s', %d, %d) -> 0\n", name ? name : "(null)", (int)a, (int)b);
            return 0;
        }
    };
    static CScheme g_Scheme;

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
            VGUI_TRACE("ISchemeManager::GetImage('%s') -> dummy image\n", imageName ? imageName : "(null)");
            return &g_DummyImage;
        }
        HTexture GetImageID(const char* imageName, bool hardwareFiltered) override
        {
            VGUI_TRACE("ISchemeManager::GetImageID('%s')\n", imageName ? imageName : "(null)");
            return 0;
        }
        void* GetIScheme(HScheme scheme) override
        {
            VGUI_TRACE("ISchemeManager::GetIScheme(%lu) -> CScheme\n", scheme);
            return &g_Scheme;
        }
        void Shutdown(bool full) override { VGUI_TRACE("ISchemeManager::Shutdown(%d)\n", (int)full); }
        int GetProportionalScaledValue(int normalizedValue) override { return normalizedValue * ScreenTall() / 480; }
        int GetProportionalNormalizedValue(int scaledValue) override { return scaledValue * 480 / ScreenTall(); }
        int Slot11() override { VGUI_TRACE("ISchemeManager::Slot11\n"); return 0; }
        // Slot 12 is called all the time by GameUI with layout constants (24, 6, -8): it is a
        // proportional scaling (base 480 -> screen height). Slot 13 is assumed to be its inverse.
        int Slot12(int a1) override { VGUI_TRACE("ISchemeManager::Slot12(%d) = scaled\n", a1); return a1 * ScreenTall() / 480; }
        int Slot13(int a1) override { VGUI_TRACE("ISchemeManager::Slot13(%d) = normalized\n", a1); return a1 * 480 / ScreenTall(); }

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
