#pragma once
#include "CPanel.h"
#include <string>

namespace vgui
{
    class CTextEntry : public CPanel
    {
    public:
        CTextEntry(HPanel parentId = INVALID_PANEL, const char* panelName = nullptr);
        virtual ~CTextEntry();
        
        void SetText(const wchar_t* text);
        const wchar_t* GetText() const { return m_Text.c_str(); }
        
        void SetMaxLength(int maxLen) { m_nMaxLength = maxLen; }
        int GetMaxLength() const { return m_nMaxLength; }
        
        void SetNumericOnly(bool state) { m_bNumericOnly = state; }
        bool IsNumericOnly() const { return m_bNumericOnly; }
        
        void SetEditable(bool state) { m_bEditable = state; }
        bool IsEditable() const { return m_bEditable; }
        
        int GetCursorPos() const { return m_nCursorPos; }
        void SetCursorPos(int pos);
        
        void Paint() override;
        void PaintBackground() override;
        
        void OnMousePressed(int code) override;
        void OnKeyCodeTyped(int code) override;
        void OnKeyCodeReleased(int code) override;
        void OnCursorMoved(int x, int y) override;
        
    protected:
        std::wstring m_Text;
        int m_nCursorPos;
        int m_nMaxLength;
        bool m_bNumericOnly;
        bool m_bEditable;
        bool m_bHasFocus;
        int m_nBlinkCounter;
    };
}
