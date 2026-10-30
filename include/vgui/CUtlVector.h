#pragma once
#include <cstdlib>
#include <cstring>

namespace vgui
{
    template<typename T>
    class CUtlVector
    {
    public:
        CUtlVector(int growSize = 0, int initSize = 0)
            : m_pData(nullptr), m_nAllocationCount(initSize), m_GrowSize(growSize), m_Size(0)
        {
            if (initSize > 0)
            {
                m_pData = (T*)malloc(initSize * sizeof(T));
            }
        }
        
        ~CUtlVector()
        {
            if (m_pData) free(m_pData);
        }
        
        T& operator[](int i) { return m_pData[i]; }
        const T& operator[](int i) const { return m_pData[i]; }
        
        int Count() const { return m_Size; }
        int NumAllocated() const { return m_nAllocationCount; }
        
        void SetSize(int size)
        {
            if (size > m_nAllocationCount)
            {
                Grow(size - m_nAllocationCount);
            }
            m_Size = size;
        }
        
        void Grow(int num = 1)
        {
            int nNewSize;
            if (m_GrowSize > 0)
            {
                nNewSize = m_nAllocationCount + m_GrowSize;
                if (nNewSize < m_nAllocationCount + num)
                    nNewSize = m_nAllocationCount + num;
            }
            else
            {
                nNewSize = m_nAllocationCount + num;
                if (nNewSize < m_nAllocationCount * 2)
                    nNewSize = m_nAllocationCount * 2;
            }
            
            if (nNewSize < m_nAllocationCount + num)
                nNewSize = m_nAllocationCount + num;
            
            T* pNewData = (T*)realloc(m_pData, nNewSize * sizeof(T));
            if (!pNewData && nNewSize > 0)
            {
                // Allocation failed
                return;
            }
            m_pData = pNewData;
            m_nAllocationCount = nNewSize;
        }
        
        int AddToTail(const T& src)
        {
            int nNewIndex = m_Size;
            if (m_Size >= m_nAllocationCount)
            {
                Grow();
            }
            m_pData[nNewIndex] = src;
            m_Size++;
            return nNewIndex;
        }
        
        int InsertBefore(int elem, const T& src)
        {
            if (elem < 0 || elem > m_Size)
            {
                // Assertion failed
                return -1;
            }
            
            if (m_Size + 1 > m_nAllocationCount)
            {
                int nNewCount;
                if (m_GrowSize > 0)
                {
                    nNewCount = m_nAllocationCount + m_GrowSize;
                    if (nNewCount < m_nAllocationCount + 1)
                        nNewCount = m_nAllocationCount + 1;
                }
                else
                {
                    nNewCount = m_nAllocationCount + 1;
                    if (nNewCount < m_nAllocationCount * 2)
                        nNewCount = m_nAllocationCount * 2;
                }
                
                if (nNewCount < m_nAllocationCount + 1)
                    nNewCount = m_nAllocationCount + 1;
                
                T* pNewData = (T*)realloc(m_pData, nNewCount * sizeof(T));
                if (!pNewData && nNewCount > 0)
                {
                    return -1;
                }
                m_pData = pNewData;
                m_nAllocationCount = nNewCount;
            }
            
            if (elem < m_Size)
            {
                memmove(&m_pData[elem + 1], &m_pData[elem], (m_Size - elem) * sizeof(T));
            }
            
            m_pData[elem] = src;
            m_Size++;
            return elem;
        }
        
        void Remove(int elem)
        {
            if (elem < 0 || elem >= m_Size) return;
            if (elem < m_Size - 1)
            {
                memmove(&m_pData[elem], &m_pData[elem + 1], (m_Size - elem - 1) * sizeof(T));
            }
            m_Size--;
        }
        
        void RemoveAll()
        {
            m_Size = 0;
        }
        
    private:
        T* m_pData;
        int m_nAllocationCount;
        int m_GrowSize;
        int m_Size;
    };
}
