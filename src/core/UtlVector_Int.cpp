#include <windows.h>
#include <cstring>

namespace vgui
{
    struct UtlVector_Int
    {
        void* m_pBuffer;
        int m_nCapacity;
        int m_nGrowSize;
        int m_nCount;
    };

    extern "C" void __fastcall UtlVector_Int_InsertBefore(UtlVector_Int* pThis, int index, const int* pValue)
    {
        if (index < 0 || index >= pThis->m_nCount)
        {
            return;
        }

        int newCount = pThis->m_nCount + 1;
        if (newCount > pThis->m_nCapacity)
        {
            int newCapacity;
            if (pThis->m_nGrowSize > 0)
            {
                newCapacity = pThis->m_nCapacity + pThis->m_nGrowSize;
                if (newCapacity < newCount)
                    newCapacity = newCount;
            }
            else
            {
                newCapacity = pThis->m_nCapacity + 1;
                if (newCapacity < pThis->m_nCapacity * 2)
                    newCapacity = pThis->m_nCapacity * 2;
            }

            if (newCapacity < newCount)
                newCapacity = newCount;

            size_t newSize = newCapacity * 4;
            void* pNewBuffer;
            if (pThis->m_pBuffer)
            {
                pNewBuffer = realloc(pThis->m_pBuffer, newSize);
            }
            else
            {
                pNewBuffer = malloc(newSize);
            }

            if (!pNewBuffer)
            {
                return;
            }

            pThis->m_pBuffer = pNewBuffer;
            pThis->m_nCapacity = newCapacity;
        }

        pThis->m_nCount++;

        if (index < pThis->m_nCount - 1)
        {
            char* pBuffer = (char*)pThis->m_pBuffer;
            char* pSrc = pBuffer + index * 4;
            char* pDst = pBuffer + (index + 1) * 4;
            size_t moveSize = (pThis->m_nCount - index - 1) * 4;

            memmove(pDst, pSrc, moveSize);
        }

        int* pArray = (int*)pThis->m_pBuffer;
        pArray[index] = *pValue;
    }
}
