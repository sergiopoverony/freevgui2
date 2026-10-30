#pragma once
#include "VGui.h"

namespace vgui
{
    struct Color
    {
        unsigned char r, g, b, a;
        Color() : r(0), g(0), b(0), a(0) {}
        Color(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa) : r(rr), g(gg), b(bb), a(aa) {}
    };

    class ISurface
    {
    public:
        virtual void Shutdown() = 0;
        virtual void RunFrame() = 0;
        virtual HPanel GetEmbeddedPanel() = 0;
        virtual void SetEmbeddedPanel(HPanel panel) = 0;
        virtual void PushMakeCurrent(HPanel panel, bool force) = 0;
        virtual void PopMakeCurrent() = 0;
        virtual void DrawSetColor(int r, int g, int b, int a) = 0;
        virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
        virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
        virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
        virtual void DrawPolyLine(int* px, int* py, int numPoints) = 0;
        virtual void DrawSetTextFont(HFont font) = 0;
        virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
        virtual void DrawSetTextPos(int x, int y) = 0;
        virtual void DrawPrintText(const wchar_t* text, int textLen) = 0;
        virtual void DrawSetTexture(int id) = 0;
        virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
        virtual int CreateNewTextureID(bool procedural) = 0;
        virtual void UnlockCursor() = 0;
        virtual void LockCursor() = 0;
        virtual HFont CreateFont() = 0;
        virtual bool SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) = 0;
        virtual void GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall) = 0;
        virtual void PlaySound(const char* fileName) = 0;
        virtual int GetPopupCount() = 0;
        virtual HPanel GetPopup(int index) = 0;
        virtual int GetModalPanel() = 0;
        virtual void SetCursor(HCursor cursor) = 0;
        virtual void SetEmbeddedWindow(void* hwnd) = 0;
        virtual void* GetEmbeddedWindow() = 0;
    };

    #define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface026"
}
