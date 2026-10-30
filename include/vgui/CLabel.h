#pragma once
#include "CPanel.h"
#include <string>

namespace vgui
{
    class CLabel : public CPanel
    {
    public:
        CLabel(HPanel parentId = INVALID_PANEL, const char* panelName = nullptr);
        virtual ~CLabel();
        
        void SetText(const wchar_t* text);
        const wchar_t* GetText() const { return m_Text.c_str(); }
        
        void SetContentAlignment(int align);
        
        void Paint() override;
        void PaintBackground() override;
        
    protected:
        std::wstring m_Text;
        int m_nContentAlignment;
    };
}
