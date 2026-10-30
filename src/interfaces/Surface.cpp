#include "vgui/ISurface.h"
#include "vgui/Trace.h"
#include <windows.h>
#include <cstring>
#include <string>
#include <vector>

namespace vgui
{
    // Surface026. This is a *logging stub with sane values*: it keeps the vtable
    // identical to the original so the engine can call every slot safely, and reports
    // which slots are actually used (vgui2_debug.log). Real drawing comes later.
    class CSurface : public ISurface
    {
    public:
        CSurface() : m_hEmbedded(0), m_nNextFont(1), m_nNextTexture(1), m_nModal(0), m_bCursorLocked(false) {}

        void Shutdown() override { VGUI_TRACE("ISurface::Shutdown\n"); }
        void RunFrame() override {}
        VPANEL GetEmbeddedPanel() override { return m_hEmbedded; }
        void SetEmbeddedPanel(VPANEL pPanel) override { VGUI_TRACE("ISurface::SetEmbeddedPanel(%lu)\n", pPanel); m_hEmbedded = pPanel; }
        void PushMakeCurrent(VPANEL panel, bool useInsets) override { VGUI_TRACE("ISurface::PushMakeCurrent(%lu,%d)\n", panel, (int)useInsets); }
        void PopMakeCurrent(VPANEL panel) override { VGUI_TRACE("ISurface::PopMakeCurrent(%lu)\n", panel); }

        void DrawSetColorC(Color col) override {}
        void DrawSetColor(int r, int g, int b, int a) override {}
        void DrawFilledRect(int x0, int y0, int x1, int y1) override {}
        void DrawOutlinedRect(int x0, int y0, int x1, int y1) override {}
        void DrawLine(int x0, int y0, int x1, int y1) override {}
        void DrawPolyLine(int* px, int* py, int numPoints) override {}
        void DrawSetTextFont(HFont font) override { m_hTextFont = font; }
        void DrawSetTextColorC(Color col) override {}
        void DrawSetTextColor(int r, int g, int b, int a) override {}
        void DrawSetTextPos(int x, int y) override { m_nTextX = x; m_nTextY = y; }
        void DrawGetTextPos(int& x, int& y) override { x = m_nTextX; y = m_nTextY; }
        void DrawPrintText(const wchar_t* text, int textLen) override {}
        void DrawUnicodeChar(wchar_t wch) override {}
        int Slot20(int a1) override { VGUI_TRACE("ISurface::Slot20(%d)\n", a1); return 0; }
        void DrawFlushText() override {}

        void* CreateHTMLWindow(void* events, VPANEL context) override { VGUI_TRACE("ISurface::CreateHTMLWindow(%p,%lu)\n", events, context); return nullptr; }
        void PaintHTMLWindow(void* htmlwin) override { VGUI_TRACE("ISurface::PaintHTMLWindow(%p)\n", htmlwin); }
        void DeleteHTMLWindow(void* htmlwin) override { VGUI_TRACE("ISurface::DeleteHTMLWindow(%p)\n", htmlwin); }

        void DrawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload) override { VGUI_TRACE("ISurface::DrawSetTextureFile(%d,'%s')\n", id, filename ? filename : "(null)"); }
        void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, int forceReload) override { VGUI_TRACE("ISurface::DrawSetTextureRGBA(%d,%dx%d)\n", id, wide, tall); }
        void DrawSetTexture(int id) override {}
        void DrawGetTextureSize(int id, int& wide, int& tall) override { wide = tall = 0; }
        void DrawTexturedRect(int x0, int y0, int x1, int y1) override {}
        bool IsTextureIDValid(int id) override { return id > 0 && id < m_nNextTexture; }
        int CreateNewTextureID(bool procedural) override { return m_nNextTexture++; }

        void GetScreenSize(int& wide, int& tall) override
        {
            wide = GetSystemMetrics(SM_CXSCREEN);
            tall = GetSystemMetrics(SM_CYSCREEN);
            if (wide <= 0) wide = 640;
            if (tall <= 0) tall = 480;
        }
        void SetAsTopMost(VPANEL panel, bool state) override {}
        void BringToFront(VPANEL panel) override {}
        void SetForegroundWindow(VPANEL panel) override {}
        void SetPanelVisible(VPANEL panel, bool state) override {}
        void SetMinimized(VPANEL panel, bool state) override {}
        bool IsMinimized(VPANEL panel) override { return false; }
        void FlashWindow(VPANEL panel, bool state) override {}
        void SetTitle(VPANEL panel, const wchar_t* title) override {}
        void SetAsToolBar(VPANEL panel, bool state) override {}
        void CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput) override { VGUI_TRACE("ISurface::CreatePopup(%lu)\n", panel); }
        void SwapBuffers(VPANEL panel) override {}
        void Invalidate(VPANEL panel) override {}
        void SetCursor(HCursor cursor) override {}
        bool IsCursorVisible() override { return true; }
        void ApplyChanges() override {}
        bool IsWithin(int x, int y) override { return true; }
        bool HasFocus() override { return true; }
        bool SupportsFeature(int feature) override { VGUI_TRACE("ISurface::SupportsFeature(%d)\n", feature); return false; }
        void RestrictPaintToSinglePanel(VPANEL panel) override {}
        void SetModalPanel(VPANEL panel) override { m_nModal = panel; }
        VPANEL GetModalPanel() override { return m_nModal; }
        void UnlockCursor() override { m_bCursorLocked = false; }
        void LockCursor() override { m_bCursorLocked = true; }
        void SetTranslateExtendedKeys(bool state) override {}
        VPANEL GetTopmostPopup() override { return 0; }
        void SetTopLevelFocus(VPANEL panel) override {}

        HFont CreateFont() override { return m_nNextFont++; }
        bool AddGlyphSetToFont(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange) override
        {
            VGUI_TRACE("ISurface::AddGlyphSetToFont(%lu,'%s',tall=%d)\n", font, windowsFontName ? windowsFontName : "(null)", tall);
            SetTall(font, tall);
            return true;
        }
        bool AddCustomFontFile(const char* fontFileName) override { VGUI_TRACE("ISurface::AddCustomFontFile('%s')\n", fontFileName ? fontFileName : "(null)"); return true; }
        int GetFontTall(HFont font) override { return FontTall(font); }
        void GetCharABCwide(HFont font, int ch, int& a, int& b, int& c) override { a = 0; b = FontTall(font) / 2; c = 0; }
        int GetCharacterWidth(HFont font, int ch) override { return FontTall(font) / 2; }
        void GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall) override
        {
            int len = text ? (int)wcslen(text) : 0;
            tall = FontTall(font);
            wide = len * (tall / 2);
        }
        VPANEL GetNotifyPanel() override { return 0; }
        void SetNotifyIcon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char* text) override {}
        void PlaySound(const char* fileName) override { VGUI_TRACE("ISurface::PlaySound('%s')\n", fileName ? fileName : "(null)"); }
        int GetPopupCount() override { return 0; }
        VPANEL GetPopup(int index) override { return 0; }
        bool ShouldPaintChildPanel(VPANEL childPanel) override { return true; }
        bool RecreateContext(VPANEL panel) override { return true; }
        void AddPanel(VPANEL panel) override { VGUI_TRACE("ISurface::AddPanel(%lu)\n", panel); }
        void ReleasePanel(VPANEL panel) override { VGUI_TRACE("ISurface::ReleasePanel(%lu)\n", panel); }
        void MovePopupToFront(VPANEL panel) override {}
        void MovePopupToBack(VPANEL panel) override {}
        void SolveTraverse(VPANEL panel, bool forceApplySchemeSettings) override { VGUI_TRACE("ISurface::SolveTraverse(%lu)\n", panel); }
        void PaintTraverse(VPANEL panel) override { VGUI_TRACE("ISurface::PaintTraverse(%lu)\n", panel); }
        void EnableMouseCapture(VPANEL panel, bool state) override {}
        void GetWorkspaceBounds(int& x, int& y, int& wide, int& tall) override { x = y = 0; GetScreenSize(wide, tall); }
        void GetAbsoluteWindowBounds(int& x, int& y, int& wide, int& tall) override { x = y = 0; GetScreenSize(wide, tall); }
        void GetProportionalBase(int& width, int& height) override { width = 640; height = 480; }
        bool CalculateMouseVisible() override { return false; }
        bool NeedKBInput() override { return false; }
        bool HasCursorPosFunctions() override { return true; }
        void SurfaceGetCursorPos(int& x, int& y) override { POINT pt = {0, 0}; ::GetCursorPos(&pt); x = pt.x; y = pt.y; }
        void SurfaceSetCursorPos(int x, int y) override { ::SetCursorPos(x, y); }

        // unnamed slots 88..106 (argument counts verified against the original)
        int Slot88(int a1, int a2) override { VGUI_TRACE("ISurface::Slot88\n"); return 0; }
        int Slot89(int a1, int a2) override { VGUI_TRACE("ISurface::Slot89\n"); return 0; }
        int Slot90(int a1) override { VGUI_TRACE("ISurface::Slot90\n"); return 0; }
        int Slot91(int a1) override { VGUI_TRACE("ISurface::Slot91\n"); return 0; }
        int Slot92() override { VGUI_TRACE("ISurface::Slot92\n"); return 0; }
        int Slot93(int a1) override { VGUI_TRACE("ISurface::Slot93\n"); return 0; }
        int Slot94(int a1, int a2, int a3, int a4, int a5, int a6) override { VGUI_TRACE("ISurface::Slot94\n"); return 0; }
        int Slot95(int a1, int a2, int a3, int a4) override { VGUI_TRACE("ISurface::Slot95\n"); return 0; }
        int Slot96(int a1, int a2, int a3, int a4) override { VGUI_TRACE("ISurface::Slot96\n"); return 0; }
        int Slot97(int a1, int a2) override { VGUI_TRACE("ISurface::Slot97\n"); return 0; }
        int Slot98() override { VGUI_TRACE("ISurface::Slot98\n"); return 0; }
        int Slot99(int a1) override { VGUI_TRACE("ISurface::Slot99\n"); return 0; }
        int Slot100(int a1) override { VGUI_TRACE("ISurface::Slot100\n"); return 0; }
        int Slot101(int a1) override { VGUI_TRACE("ISurface::Slot101\n"); return 0; }
        int Slot102(int a1) override { VGUI_TRACE("ISurface::Slot102\n"); return 0; }
        int Slot103(int a1, int a2) override { VGUI_TRACE("ISurface::Slot103\n"); return 0; }
        int Slot104(int a1, int a2) override { VGUI_TRACE("ISurface::Slot104\n"); return 0; }
        int Slot105(int a1, int a2) override { VGUI_TRACE("ISurface::Slot105\n"); return 0; }
        int Slot106(int a1) override { VGUI_TRACE("ISurface::Slot106\n"); return 0; }

    private:
        int FontTall(HFont font) const
        {
            return (font >= 1 && font <= m_Tall.size() && m_Tall[font - 1] > 0) ? m_Tall[font - 1] : 16;
        }
        void SetTall(HFont font, int tall)
        {
            if (font < 1 || font > 4096) return;
            if (m_Tall.size() < font) m_Tall.resize(font, 0);
            m_Tall[font - 1] = tall;
        }

        VPANEL m_hEmbedded;
        HFont m_nNextFont;
        int m_nNextTexture;
        VPANEL m_nModal;
        bool m_bCursorLocked;
        HFont m_hTextFont = 0;
        int m_nTextX = 0, m_nTextY = 0;
        std::vector<int> m_Tall;
    };

    static CSurface g_SurfaceInstance;

    void* CreateSurfaceReal()
    {
        Log("CreateSurfaceReal() returning %p\n", &g_SurfaceInstance);
        return &g_SurfaceInstance;
    }
}
