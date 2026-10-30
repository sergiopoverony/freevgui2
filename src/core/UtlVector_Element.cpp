namespace vgui
{
    struct UtlVector_Int
    {
        void* m_pBuffer;
        int m_nCapacity;
        int m_nGrowSize;
        int m_nCount;
    };

    extern "C" void* __fastcall UtlVector_Int_Element(UtlVector_Int* pThis, int index)
    {
        if (index < 0 || index >= pThis->m_nCount)
        {
            return nullptr;
        }

        char* pBuffer = (char*)pThis->m_pBuffer;
        return pBuffer + index * 4;
    }
}
