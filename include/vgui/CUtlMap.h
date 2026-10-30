#pragma once
#include <cstdlib>
#include <cstring>

namespace vgui
{
    template<typename K, typename V>
    class CUtlMap
    {
    public:
        struct Node
        {
            K m_Key;
            V m_Value;
            int m_Next;
            
            Node() : m_Next(-1) {}
        };
        
        typedef bool (*LessFunc_t)(const K&, const K&);
        
        CUtlMap(int growSize = 0, LessFunc_t lessFunc = nullptr)
            : m_pElements(nullptr)
            , m_nCapacity(0)
            , m_nCount(0)
            , m_pBuckets(nullptr)
            , m_nBucketCount(0)
            , m_LessFunc(lessFunc)
        {
            if (growSize > 0)
            {
                Grow(growSize);
            }
        }
        
        ~CUtlMap()
        {
            if (m_pElements) free(m_pElements);
            if (m_pBuckets) free(m_pBuckets);
        }
        
        int FindOrCreate(const K& key)
        {
            if (m_nBucketCount == 0)
            {
                Grow(16);
            }
            
            unsigned int hash = Hash(key);
            int bucket = hash % m_nBucketCount;
            
            int idx = m_pBuckets[bucket];
            while (idx != -1)
            {
                if (m_LessFunc ? (!m_LessFunc(key, m_pElements[idx].m_Key) && !m_LessFunc(m_pElements[idx].m_Key, key)) : (key == m_pElements[idx].m_Key))
                {
                    return idx;
                }
                idx = m_pElements[idx].m_Next;
            }
            
            if (m_nCount >= m_nCapacity)
            {
                Grow(m_nCapacity > 0 ? m_nCapacity * 2 : 16);
            }
            
            int newIdx = m_nCount++;
            m_pElements[newIdx].m_Key = key;
            m_pElements[newIdx].m_Next = m_pBuckets[bucket];
            m_pBuckets[bucket] = newIdx;
            
            return newIdx;
        }
        
        int Find(const K& key) const
        {
            if (m_nBucketCount == 0) return -1;
            
            unsigned int hash = Hash(key);
            int bucket = hash % m_nBucketCount;
            
            int idx = m_pBuckets[bucket];
            while (idx != -1)
            {
                if (m_LessFunc ? (!m_LessFunc(key, m_pElements[idx].m_Key) && !m_LessFunc(m_pElements[idx].m_Key, key)) : (key == m_pElements[idx].m_Key))
                {
                    return idx;
                }
                idx = m_pElements[idx].m_Next;
            }
            
            return -1;
        }
        
        V& Element(int idx) { return m_pElements[idx].m_Value; }
        const V& Element(int idx) const { return m_pElements[idx].m_Value; }
        
        int Count() const { return m_nCount; }
        
    private:
        void Grow(int newCapacity)
        {
            Node* pNewElements = (Node*)realloc(m_pElements, newCapacity * sizeof(Node));
            if (!pNewElements) return;
            
            m_pElements = pNewElements;
            m_nCapacity = newCapacity;
            
            int newBucketCount = newCapacity;
            int* pNewBuckets = (int*)malloc(newBucketCount * sizeof(int));
            if (!pNewBuckets) return;
            
            for (int i = 0; i < newBucketCount; i++)
            {
                pNewBuckets[i] = -1;
            }
            
            for (int i = 0; i < m_nCount; i++)
            {
                unsigned int hash = Hash(m_pElements[i].m_Key);
                int bucket = hash % newBucketCount;
                m_pElements[i].m_Next = pNewBuckets[bucket];
                pNewBuckets[bucket] = i;
            }
            
            if (m_pBuckets) free(m_pBuckets);
            m_pBuckets = pNewBuckets;
            m_nBucketCount = newBucketCount;
        }
        
        unsigned int Hash(const K& key) const
        {
            const unsigned char* p = (const unsigned char*)&key;
            unsigned int hash = 0;
            for (size_t i = 0; i < sizeof(K); i++)
            {
                hash = hash * 31 + p[i];
            }
            return hash;
        }
        
        Node* m_pElements;
        int m_nCapacity;
        int m_nCount;
        int* m_pBuckets;
        int m_nBucketCount;
        LessFunc_t m_LessFunc;
    };
}
