#pragma once
#include "VGui.h"

namespace vgui
{
    typedef unsigned long VPANEL;

    // VGUI_Panel007. The order of virtual methods MUST match the original
    // vtable. Slot 0 is the virtual destructor (IBaseInterface) and slot 1 is
    // Init(VPANEL, IClientPanel*): confirmed by the game log, where the engine's
    // first call arrived as (1, <pointer>, <garbage>) into what we had as SetPos.
    // Everything below slot 1 follows the Source SDK layout; verify against the
    // original vgui2.dll and the "IPanel::" lines of vgui2_debug.log.
    class IPanel
    {
    public:
        virtual ~IPanel() {}                                                                   // 0
        virtual void Init(VPANEL vguiPanel, void* clientPanel) = 0;                            // 1
        virtual void SetPos(VPANEL vguiPanel, int x, int y) = 0;                               // 2
        virtual void GetPos(VPANEL vguiPanel, int& x, int& y) = 0;                             // 3
        virtual void SetSize(VPANEL vguiPanel, int wide, int tall) = 0;                        // 4
        virtual void GetSize(VPANEL vguiPanel, int& wide, int& tall) = 0;                      // 5
        virtual void SetMinimumSize(VPANEL vguiPanel, int wide, int tall) = 0;                 // 6
        virtual void GetMinimumSize(VPANEL vguiPanel, int& wide, int& tall) = 0;               // 7
        virtual void SetZPos(VPANEL vguiPanel, int z) = 0;                                     // 8
        virtual int GetZPos(VPANEL vguiPanel) = 0;                                             // 9
        virtual void GetAbsPos(VPANEL vguiPanel, int& x, int& y) = 0;                          // 10
        virtual void GetClipRect(VPANEL vguiPanel, int& x0, int& y0, int& x1, int& y1) = 0;    // 11
        virtual void SetInset(VPANEL vguiPanel, int left, int top, int right, int bottom) = 0; // 12
        virtual void GetInset(VPANEL vguiPanel, int& left, int& top, int& right, int& bottom) = 0; // 13
        virtual void SetVisible(VPANEL vguiPanel, bool state) = 0;                             // 14
        virtual bool IsVisible(VPANEL vguiPanel) = 0;                                          // 15
        virtual void SetParent(VPANEL vguiPanel, VPANEL newParent) = 0;                        // 16
        virtual int GetChildCount(VPANEL vguiPanel) = 0;                                       // 17
        virtual VPANEL GetChild(VPANEL vguiPanel, int index) = 0;                              // 18
        virtual VPANEL GetParent(VPANEL vguiPanel) = 0;                                        // 19
        virtual void MoveToFront(VPANEL vguiPanel) = 0;                                        // 20
        virtual void MoveToBack(VPANEL vguiPanel) = 0;                                         // 21
        virtual bool HasParent(VPANEL vguiPanel, VPANEL potentialParent) = 0;                  // 22
        virtual bool IsPopup(VPANEL vguiPanel) = 0;                                            // 23
        virtual void SetPopup(VPANEL vguiPanel, bool state) = 0;                               // 24
        virtual bool Render_GetPopupVisible(VPANEL vguiPanel) = 0;                             // 25
        virtual void Render_SetPopupVisible(VPANEL vguiPanel, bool state) = 0;                 // 26
        virtual HScheme GetScheme(VPANEL vguiPanel) = 0;                                       // 27
        virtual bool IsProportional(VPANEL vguiPanel) = 0;                                     // 28
        virtual bool IsAutoDeleteSet(VPANEL vguiPanel) = 0;                                    // 29
        virtual void DeletePanel(VPANEL vguiPanel) = 0;                                        // 30
        virtual void SetKeyBoardInputEnabled(VPANEL vguiPanel, bool state) = 0;                // 31
        virtual void SetMouseInputEnabled(VPANEL vguiPanel, bool state) = 0;                   // 32
        virtual bool IsKeyBoardInputEnabled(VPANEL vguiPanel) = 0;                             // 33
        virtual bool IsMouseInputEnabled(VPANEL vguiPanel) = 0;                                // 34
        virtual void Solve(VPANEL vguiPanel) = 0;                                              // 35
        virtual const char* GetName(VPANEL vguiPanel) = 0;                                     // 36
        virtual const char* GetClassName(VPANEL vguiPanel) = 0;                                // 37
        virtual void SendMessage(VPANEL vguiPanel, void* params, VPANEL ifromPanel) = 0;       // 38
        virtual void Think(VPANEL vguiPanel) = 0;                                              // 39
        virtual void PerformApplySchemeSettings(VPANEL vguiPanel) = 0;                         // 40
        virtual void PaintTraverse(VPANEL vguiPanel, bool forceRepaint, bool allowForce) = 0;  // 41
        virtual void Repaint(VPANEL vguiPanel) = 0;                                            // 42
        virtual VPANEL IsWithinTraverse(VPANEL vguiPanel, int x, int y, bool traversePopups) = 0; // 43
        virtual void OnChildAdded(VPANEL vguiPanel, VPANEL child) = 0;                         // 44
        virtual void OnSizeChanged(VPANEL vguiPanel, int newWide, int newTall) = 0;            // 45
        virtual void InternalFocusChanged(VPANEL vguiPanel, bool lost) = 0;                    // 46
        virtual bool RequestInfo(VPANEL vguiPanel, void* outputData) = 0;                      // 47
        virtual void RequestFocus(VPANEL vguiPanel, int direction) = 0;                        // 48
        virtual bool RequestFocusPrev(VPANEL vguiPanel, VPANEL existingPanel) = 0;             // 49
        virtual bool RequestFocusNext(VPANEL vguiPanel, VPANEL existingPanel) = 0;             // 50
        virtual VPANEL GetCurrentKeyFocus(VPANEL vguiPanel) = 0;                               // 51
        virtual int GetTabPosition(VPANEL vguiPanel) = 0;                                      // 52
        virtual void* Plat(VPANEL vguiPanel) = 0;                                              // 53
        virtual void SetPlat(VPANEL vguiPanel, void* plat) = 0;                                // 54
        virtual void* GetPanel(VPANEL vguiPanel, const char* destinationModule) = 0;           // 55
        virtual bool IsEnabled(VPANEL vguiPanel) = 0;                                          // 56
        virtual void SetEnabled(VPANEL vguiPanel, bool state) = 0;                             // 57
    };

    #define VGUI_PANEL_INTERFACE_VERSION "VGUI_Panel007"
}
