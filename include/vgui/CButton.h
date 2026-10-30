#pragma once
#include "CPanel.h"
#include <string>

namespace vgui
{
    class CButton : public CPanel
    {
    public:
        CButton(HPanel parentId = INVALID_PANEL, const char* panelName = nullptr);
        virtual ~CButton();
        
        void SetText(const wchar_t* text);
        const wchar_t* GetText() const { return m_Text.c_str(); }
        
        void SetCommand(const char* command);
        const char* GetCommand() const { return m_Command.c_str(); }
        
        void SetArmed(bool state) { m_bArmed = state; Repaint(); }
        bool IsArmed() const { return m_bArmed; }
        
        void SetDepressed(bool state) { m_bDepressed = state; Repaint(); }
        bool IsDepressed() const { return m_bDepressed; }
        
        void Paint() override;
        void PaintBackground() override;
        
        void OnMousePressed(int code) override;
        void OnMouseReleased(int code) override;
        void OnCursorMoved(int x, int y) override;
        void OnCursorEntered() override;
        void OnCursorExited() override;
        void OnKeyCodeTyped(int code) override;
        
    protected:
        std::wstring m_Text;
        std::string m_Command;
        bool m_bArmed;
        bool m_bDepressed;
    };
}
