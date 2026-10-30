#include "vgui/ISurface.h"
#include <windows.h>
#include <map>
#include <string>
#include <vector>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#undef CreateFont
#undef PlaySound

namespace vgui
{
    struct FontInfo
    {
        HFONT hFont;
        int tall;
        int weight;
        std::string name;
    };

    struct TextureInfo
    {
        HDC hMemDC;
        HBITMAP hBitmap;
        int width;
        int height;
    };

    class CSurface : public ISurface
    {
    public:
        CSurface()
            : m_hWnd(nullptr)
            , m_hDC(nullptr)
            , m_nEmbeddedPanel(INVALID_PANEL)
            , m_nCurrentFont(INVALID_FONT)
            , m_nCurrentTexture(0)
            , m_nNextTextureID(1)
            , m_nNextFontID(1)
            , m_nTextX(0), m_nTextY(0)
            , m_bCursorLocked(false)
        {
            m_DrawColor.r = 255;
            m_DrawColor.g = 255;
            m_DrawColor.b = 255;
            m_DrawColor.a = 255;
            m_TextColor.r = 255;
            m_TextColor.g = 255;
            m_TextColor.b = 255;
            m_TextColor.a = 255;
        }

        ~CSurface()
        {
            Shutdown();
        }

        void Shutdown() override
        {
            for (auto& pair : m_Fonts)
            {
                if (pair.second.hFont) DeleteObject(pair.second.hFont);
            }
            m_Fonts.clear();

            for (auto& pair : m_Textures)
            {
                if (pair.second.hBitmap) DeleteObject(pair.second.hBitmap);
                if (pair.second.hMemDC) DeleteDC(pair.second.hMemDC);
            }
            m_Textures.clear();

            m_hWnd = nullptr;
            m_hDC = nullptr;
        }

        void RunFrame() override
        {
        }

        HPanel GetEmbeddedPanel() override
        {
            return m_nEmbeddedPanel;
        }

        void SetEmbeddedPanel(HPanel panel) override
        {
            m_nEmbeddedPanel = panel;
        }

        void PushMakeCurrent(HPanel panel, bool force) override
        {
        }

        void PopMakeCurrent() override
        {
        }

        void DrawSetColor(int r, int g, int b, int a) override
        {
            m_DrawColor.r = (unsigned char)r;
            m_DrawColor.g = (unsigned char)g;
            m_DrawColor.b = (unsigned char)b;
            m_DrawColor.a = (unsigned char)a;
        }

        void DrawFilledRect(int x0, int y0, int x1, int y1) override
        {
            if (!m_hDC) return;
            COLORREF color = RGB(m_DrawColor.r, m_DrawColor.g, m_DrawColor.b);
            HBRUSH hBrush = CreateSolidBrush(color);
            RECT rc = { x0, y0, x1, y1 };
            FillRect(m_hDC, &rc, hBrush);
            DeleteObject(hBrush);
        }

        void DrawOutlinedRect(int x0, int y0, int x1, int y1) override
        {
            if (!m_hDC) return;
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(m_DrawColor.r, m_DrawColor.g, m_DrawColor.b));
            HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, GetStockObject(NULL_BRUSH));
            HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
            Rectangle(m_hDC, x0, y0, x1, y1);
            SelectObject(m_hDC, hOldPen);
            SelectObject(m_hDC, hOldBrush);
            DeleteObject(hPen);
        }

        void DrawLine(int x0, int y0, int x1, int y1) override
        {
            if (!m_hDC) return;
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(m_DrawColor.r, m_DrawColor.g, m_DrawColor.b));
            HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
            MoveToEx(m_hDC, x0, y0, nullptr);
            LineTo(m_hDC, x1, y1);
            SelectObject(m_hDC, hOldPen);
            DeleteObject(hPen);
        }

        void DrawPolyLine(int* px, int* py, int numPoints) override
        {
            if (!m_hDC || numPoints < 2) return;
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(m_DrawColor.r, m_DrawColor.g, m_DrawColor.b));
            HPEN hOldPen = (HPEN)SelectObject(m_hDC, hPen);
            MoveToEx(m_hDC, px[0], py[0], nullptr);
            for (int i = 1; i < numPoints; i++)
            {
                LineTo(m_hDC, px[i], py[i]);
            }
            SelectObject(m_hDC, hOldPen);
            DeleteObject(hPen);
        }

        void DrawSetTextFont(HFont font) override
        {
            m_nCurrentFont = font;
        }

        void DrawSetTextColor(int r, int g, int b, int a) override
        {
            m_TextColor.r = (unsigned char)r;
            m_TextColor.g = (unsigned char)g;
            m_TextColor.b = (unsigned char)b;
            m_TextColor.a = (unsigned char)a;
        }

        void DrawSetTextPos(int x, int y) override
        {
            m_nTextX = x;
            m_nTextY = y;
        }

        void DrawPrintText(const wchar_t* text, int textLen) override
        {
            if (!m_hDC || !text) return;

            auto it = m_Fonts.find(m_nCurrentFont);
            HFONT hFont = nullptr;
            if (it != m_Fonts.end())
            {
                hFont = it->second.hFont;
            }

            HFONT hOldFont = (HFONT)SelectObject(m_hDC, hFont ? hFont : (HFONT)GetStockObject(DEFAULT_GUI_FONT));
            SetTextColor(m_hDC, RGB(m_TextColor.r, m_TextColor.g, m_TextColor.b));
            SetBkMode(m_hDC, TRANSPARENT);
            TextOutW(m_hDC, m_nTextX, m_nTextY, text, textLen >= 0 ? textLen : (int)wcslen(text));
            SelectObject(m_hDC, hOldFont);

            SIZE size;
            GetTextExtentPoint32W(m_hDC, text, textLen >= 0 ? textLen : (int)wcslen(text), &size);
            m_nTextX += size.cx;
        }

        void DrawSetTexture(int id) override
        {
            m_nCurrentTexture = id;
        }

        void DrawTexturedRect(int x0, int y0, int x1, int y1) override
        {
            auto it = m_Textures.find(m_nCurrentTexture);
            if (it == m_Textures.end() || !m_hDC) return;

            BitBlt(m_hDC, x0, y0, x1 - x0, y1 - y0, it->second.hMemDC, 0, 0, SRCCOPY);
        }

        int CreateNewTextureID(bool procedural) override
        {
            int id = m_nNextTextureID++;
            TextureInfo info = { nullptr, nullptr, 0, 0 };
            m_Textures[id] = info;
            return id;
        }

        void UnlockCursor() override
        {
            if (m_bCursorLocked)
            {
                ShowCursor(TRUE);
                m_bCursorLocked = false;
            }
        }

        void LockCursor() override
        {
            if (!m_bCursorLocked)
            {
                ShowCursor(FALSE);
                m_bCursorLocked = true;
            }
        }

        HFont CreateFont() override
        {
            HFont id = m_nNextFontID++;
            FontInfo info = { nullptr, 0, 0, "" };
            m_Fonts[id] = info;
            return id;
        }

        bool SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) override
        {
            if (!windowsFontName) return false;

            auto it = m_Fonts.find(font);
            if (it == m_Fonts.end()) return false;

            if (it->second.hFont)
            {
                DeleteObject(it->second.hFont);
            }

            HFONT hFont = ::CreateFontA(
                tall, 0, 0, 0,
                weight,
                (flags & 0x01) ? TRUE : FALSE,
                (flags & 0x02) ? TRUE : FALSE,
                (flags & 0x04) ? TRUE : FALSE,
                DEFAULT_CHARSET,
                OUT_TT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE,
                windowsFontName
            );

            if (!hFont) return false;

            it->second.hFont = hFont;
            it->second.tall = tall;
            it->second.weight = weight;
            it->second.name = windowsFontName;

            return true;
        }

        void GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall) override
        {
            wide = tall = 0;
            if (!text || !m_hDC) return;

            HFONT hFont = nullptr;
            auto it = m_Fonts.find(font);
            if (it != m_Fonts.end())
            {
                hFont = it->second.hFont;
            }

            HFONT hOldFont = (HFONT)SelectObject(m_hDC, hFont ? hFont : (HFONT)GetStockObject(DEFAULT_GUI_FONT));
            SIZE size;
            GetTextExtentPoint32W(m_hDC, text, (int)wcslen(text), &size);
            wide = size.cx;
            tall = size.cy;
            SelectObject(m_hDC, hOldFont);
        }

        void PlaySound(const char* fileName) override
        {
            if (fileName)
            {
                ::PlaySoundA(fileName, nullptr, SND_FILENAME | SND_ASYNC);
            }
        }

        int GetPopupCount() override
        {
            return (int)m_Popups.size();
        }

        HPanel GetPopup(int index) override
        {
            if (index < 0 || index >= (int)m_Popups.size()) return INVALID_PANEL;
            return m_Popups[index];
        }

        int GetModalPanel() override
        {
            return INVALID_PANEL;
        }

        void SetCursor(HCursor cursor) override
        {
        }

        void SetEmbeddedWindow(void* hwnd) override
        {
            m_hWnd = (HWND)hwnd;
            if (m_hWnd)
            {
                m_hDC = GetDC(m_hWnd);
            }
        }

        void* GetEmbeddedWindow() override
        {
            return m_hWnd;
        }

    private:
        HWND m_hWnd;
        HDC m_hDC;
        HPanel m_nEmbeddedPanel;
        HFont m_nCurrentFont;
        int m_nCurrentTexture;
        int m_nNextTextureID;
        HFont m_nNextFontID;
        int m_nTextX, m_nTextY;
        bool m_bCursorLocked;
        Color m_DrawColor;
        Color m_TextColor;
        std::map<HFont, FontInfo> m_Fonts;
        std::map<int, TextureInfo> m_Textures;
        std::vector<HPanel> m_Popups;
    };

    static CSurface g_SurfaceInstance;

    void* CreateSurfaceReal()
    {
        return &g_SurfaceInstance;
    }
}
