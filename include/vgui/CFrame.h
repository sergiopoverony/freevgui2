#pragma once
#include "CPanel.h"
#include <string>

namespace vgui
{
    class CFrame : public CPanel
    {
    public:
        CFrame(HPanel parentId = INVALID_PANEL, const char* panelName = nullptr);
        virtual ~CFrame();
        
        void SetTitle(const wchar_t* title);
        const wchar_t* GetTitle() const { return m_Title.c_str(); }
        
        void SetSizeable(bool state) { m_bSizeable = state; }
        bool IsSizeable() const { return m_bSizeable; }
        
        void SetMoveable(bool state) { m_bMoveable = state; }
        bool IsMoveable() const { return m_bMoveable; }
        
        void SetCloseButtonVisible(bool state) { m_bCloseButtonVisible = state; }
        bool IsCloseButtonVisible() const { return m_bCloseButtonVisible; }
        
        void SetMinimizeButtonVisible(bool state) { m_bMinimizeButtonVisible = state; }
        bool IsMinimizeButtonVisible() const { return m_bMinimizeButtonVisible; }
        
        void Activate();
        void Close();
        
        void Paint() override;
        void PaintBackground() override;
        void PerformLayout() override;
        
        void OnMousePressed(int code) override;
        void OnMouseReleased(int code) override;
        void OnCursorMoved(int x, int y) override;
        void OnKeyCodeTyped(int code) override;
        
    protected:
        std::wstring m_Title;
        bool m_bSizeable;
        bool m_bMoveable;
        bool m_bCloseButtonVisible;
        bool m_bMinimizeButtonVisible;
        bool m_bDragging;
        int m_nDragStartX, m_nDragStartY;
        int m_nPanelStartX, m_nPanelStartY;
    };
}
