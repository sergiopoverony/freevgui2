#pragma once
#include "VGui.h"

namespace vgui
{
    // 4 bytes, passed by value like a DWORD (same as the original).
    struct Color
    {
        unsigned char r, g, b, a;
        Color() : r(0), g(0), b(0), a(0) {}
        Color(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa) : r(rr), g(gg), b(bb), a(aa) {}
    };

    // VGUI_Surface026: 107 slots, verified against the original vgui2.dll
    // (see original/vtable_signatures.txt: slot index and argument count).
    // Names follow the Source SDK up to slot 87; the rest are unnamed placeholders
    // that only guarantee the right number of stack arguments.
    // Overloads have distinct names (DrawSetColorC ...) because MSVC reorders
    // overloaded virtuals in the vtable.
    class ISurface
    {
    public:
        virtual ~ISurface() {}                                                                                                   // 0
        virtual void Shutdown() = 0;                                                                                             // 1
        virtual void RunFrame() = 0;                                                                                             // 2
        virtual VPANEL GetEmbeddedPanel() = 0;                                                                                   // 3
        virtual void SetEmbeddedPanel(VPANEL pPanel) = 0;                                                                        // 4
        virtual void PushMakeCurrent(VPANEL panel, bool useInsets) = 0;                                                          // 5
        virtual void PopMakeCurrent(VPANEL panel) = 0;                                                                           // 6
        virtual void DrawSetColorC(Color col) = 0;                                                                               // 7
        virtual void DrawSetColor(int r, int g, int b, int a) = 0;                                                               // 8
        virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;                                                         // 9
        virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;                                                       // 10
        virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;                                                               // 11
        virtual void DrawPolyLine(int* px, int* py, int numPoints) = 0;                                                          // 12
        virtual void DrawSetTextFont(HFont font) = 0;                                                                            // 13
        virtual void DrawSetTextColorC(Color col) = 0;                                                                           // 14
        virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;                                                           // 15
        virtual void DrawSetTextPos(int x, int y) = 0;                                                                           // 16
        virtual void DrawGetTextPos(int& x, int& y) = 0;                                                                         // 17
        virtual void DrawPrintText(const wchar_t* text, int textLen) = 0;                                                        // 18
        virtual void DrawUnicodeChar(wchar_t wch) = 0;                                                                           // 19
        virtual int Slot20(int a1) = 0;                                                                                         // 20
        virtual void DrawFlushText() = 0;                                                                                        // 21
        virtual void* CreateHTMLWindow(void* events, VPANEL context) = 0;                                                        // 22
        virtual void PaintHTMLWindow(void* htmlwin) = 0;                                                                         // 23
        virtual void DeleteHTMLWindow(void* htmlwin) = 0;                                                                        // 24
        virtual void DrawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload) = 0;                 // 25
        virtual void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, int forceReload) = 0; // 26
        virtual void DrawSetTexture(int id) = 0;                                                                                 // 27
        virtual void DrawGetTextureSize(int id, int& wide, int& tall) = 0;                                                       // 28
        virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;                                                       // 29
        virtual bool IsTextureIDValid(int id) = 0;                                                                               // 30
        virtual int CreateNewTextureID(bool procedural) = 0;                                                                     // 31
        virtual void GetScreenSize(int& wide, int& tall) = 0;                                                                    // 32
        virtual void SetAsTopMost(VPANEL panel, bool state) = 0;                                                                 // 33
        virtual void BringToFront(VPANEL panel) = 0;                                                                             // 34
        virtual void SetForegroundWindow(VPANEL panel) = 0;                                                                      // 35
        virtual void SetPanelVisible(VPANEL panel, bool state) = 0;                                                              // 36
        virtual void SetMinimized(VPANEL panel, bool state) = 0;                                                                 // 37
        virtual bool IsMinimized(VPANEL panel) = 0;                                                                              // 38
        virtual void FlashWindow(VPANEL panel, bool state) = 0;                                                                  // 39
        virtual void SetTitle(VPANEL panel, const wchar_t* title) = 0;                                                           // 40
        virtual void SetAsToolBar(VPANEL panel, bool state) = 0;                                                                 // 41
        virtual void CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput) = 0; // 42
        virtual void SwapBuffers(VPANEL panel) = 0;                                                                              // 43
        virtual void Invalidate(VPANEL panel) = 0;                                                                               // 44
        virtual void SetCursor(HCursor cursor) = 0;                                                                              // 45
        virtual bool IsCursorVisible() = 0;                                                                                      // 46
        virtual void ApplyChanges() = 0;                                                                                         // 47
        virtual bool IsWithin(int x, int y) = 0;                                                                                 // 48
        virtual bool HasFocus() = 0;                                                                                             // 49
        virtual bool SupportsFeature(int feature) = 0;                                                                           // 50
        virtual void RestrictPaintToSinglePanel(VPANEL panel) = 0;                                                               // 51
        virtual void SetModalPanel(VPANEL panel) = 0;                                                                            // 52
        virtual VPANEL GetModalPanel() = 0;                                                                                      // 53
        virtual void UnlockCursor() = 0;                                                                                         // 54
        virtual void LockCursor() = 0;                                                                                           // 55
        virtual void SetTranslateExtendedKeys(bool state) = 0;                                                                   // 56
        virtual VPANEL GetTopmostPopup() = 0;                                                                                    // 57
        virtual void SetTopLevelFocus(VPANEL panel) = 0;                                                                         // 58
        virtual HFont CreateFont() = 0;                                                                                          // 59
        virtual bool AddGlyphSetToFont(HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange) = 0; // 60
        virtual bool AddCustomFontFile(const char* fontFileName) = 0;                                                            // 61
        virtual int GetFontTall(HFont font) = 0;                                                                                 // 62
        virtual void GetCharABCwide(HFont font, int ch, int& a, int& b, int& c) = 0;                                             // 63
        virtual int GetCharacterWidth(HFont font, int ch) = 0;                                                                   // 64
        virtual void GetTextSize(HFont font, const wchar_t* text, int& wide, int& tall) = 0;                                     // 65
        virtual VPANEL GetNotifyPanel() = 0;                                                                                     // 66
        virtual void SetNotifyIcon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char* text) = 0;          // 67
        virtual void PlaySound(const char* fileName) = 0;                                                                        // 68
        virtual int GetPopupCount() = 0;                                                                                         // 69
        virtual VPANEL GetPopup(int index) = 0;                                                                                  // 70
        virtual bool ShouldPaintChildPanel(VPANEL childPanel) = 0;                                                               // 71
        virtual bool RecreateContext(VPANEL panel) = 0;                                                                          // 72
        virtual void AddPanel(VPANEL panel) = 0;                                                                                 // 73
        virtual void ReleasePanel(VPANEL panel) = 0;                                                                             // 74
        virtual void MovePopupToFront(VPANEL panel) = 0;                                                                         // 75
        virtual void MovePopupToBack(VPANEL panel) = 0;                                                                          // 76
        virtual void SolveTraverse(VPANEL panel, bool forceApplySchemeSettings) = 0;                                             // 77
        virtual void PaintTraverse(VPANEL panel) = 0;                                                                            // 78
        virtual void EnableMouseCapture(VPANEL panel, bool state) = 0;                                                           // 79
        virtual void GetWorkspaceBounds(int& x, int& y, int& wide, int& tall) = 0;                                               // 80
        virtual void GetAbsoluteWindowBounds(int& x, int& y, int& wide, int& tall) = 0;                                          // 81
        virtual void GetProportionalBase(int& width, int& height) = 0;                                                           // 82
        virtual bool CalculateMouseVisible() = 0;                                                                                // 83
        virtual bool NeedKBInput() = 0;                                                                                          // 84
        virtual bool HasCursorPosFunctions() = 0;                                                                                // 85
        virtual void SurfaceGetCursorPos(int& x, int& y) = 0;                                                                    // 86
        virtual void SurfaceSetCursorPos(int x, int y) = 0;                                                                      // 87
        virtual int Slot88(int a1, int a2) = 0;                                                                                  // 88
        virtual int Slot89(int a1, int a2) = 0;                                                                                  // 89
        virtual int Slot90(int a1) = 0;                                                                                          // 90
        virtual int Slot91(int a1) = 0;                                                                                          // 91
        virtual int Slot92() = 0;                                                                                                // 92
        virtual int Slot93(int a1) = 0;                                                                                          // 93
        virtual int Slot94(int a1, int a2, int a3, int a4, int a5, int a6) = 0;                                                  // 94
        virtual int Slot95(int a1, int a2, int a3, int a4) = 0;                                                                  // 95
        virtual int Slot96(int a1, int a2, int a3, int a4) = 0;                                                                  // 96
        virtual int Slot97(int a1, int a2) = 0;                                                                                  // 97
        virtual int Slot98() = 0;                                                                                                // 98
        virtual int Slot99(int a1) = 0;                                                                                          // 99
        virtual int Slot100(int a1) = 0;                                                                                         // 100
        virtual int Slot101(int a1) = 0;                                                                                         // 101
        virtual int Slot102(int a1) = 0;                                                                                         // 102
        virtual int Slot103(int a1, int a2) = 0;                                                                                 // 103
        virtual int Slot104(int a1, int a2) = 0;                                                                                 // 104
        virtual int Slot105(int a1, int a2) = 0;                                                                                 // 105
        virtual int Slot106(int a1) = 0;                                                                                         // 106
    };

    #define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface026"
}
