#include <windows.h>
#include <cstring>
#include "vgui/CUtlVector.h"

namespace vgui
{
    // Структура для 12-байтовых элементов (3 DWORD)
    struct UtlVector_12Byte
    {
        void* m_pBuffer;      // +0x00
        int m_nCapacity;      // +0x04
        int m_nGrowSize;      // +0x08
        int m_nCount;         // +0x0C
    };

    // Функция 0x1001F262 - memmove wrapper
    extern "C" void* __cdecl MemMove_1001F262(void* dst, const void* src, size_t size)
    {
        return memmove(dst, src, size);
    }

    // Функция 0x1000a480 - InsertBefore для 12-байтовых элементов
    // thiscall: ecx = this, [ebp+8] = index
    // Возвращает: указатель на вставленный элемент
    extern "C" void* __fastcall UtlVector_12Byte_InsertBefore(UtlVector_12Byte* pThis, int index)
    {
        // Проверка индекса
        if (index < 0 || index >= pThis->m_nCount)
        {
            // Assertion failed
            return nullptr;
        }

        // Вычисление нового размера
        int newCount = pThis->m_nCount + 1;
        if (newCount > pThis->m_nCapacity)
        {
            // Нужно расти
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

            // Реаллокация: newCapacity * 12 байт
            size_t newSize = newCapacity * 12;
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
                // Allocation failed
                return nullptr;
            }

            pThis->m_pBuffer = pNewBuffer;
            pThis->m_nCapacity = newCapacity;
        }

        // Увеличение count
        pThis->m_nCount++;

        // Если вставка не в конец, нужно сдвинуть элементы
        if (index < pThis->m_nCount - 1)
        {
            // Вычисление адресов
            char* pBuffer = (char*)pThis->m_pBuffer;
            char* pSrc = pBuffer + index * 12;
            char* pDst = pBuffer + (index + 1) * 12;
            size_t moveSize = (pThis->m_nCount - index - 1) * 12;

            // Сдвиг элементов
            MemMove_1001F262(pDst, pSrc, moveSize);
        }

        // Возврат указателя на вставленный элемент
        return (char*)pThis->m_pBuffer + index * 12;
    }
}