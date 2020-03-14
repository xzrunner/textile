#pragma once

#include "textile/Page.h"

#include <boost/noncopyable.hpp>
#include <unordered_map>

namespace textile
{

class PageLoader;
class PageIndexer;

class PageCache : public boost::noncopyable
{
public:
    PageCache(PageLoader& loader,
        const PageIndexer& indexer);

    virtual void LoadComplete(const Page& page, const uint8_t* data) {}

    bool Touch(const Page& page);

    bool Request(const Page& page);

    void Clear() { m_lru.Clear(); }

private:
    struct CachePage
    {
        Page page;
        int x, y;

        CachePage *prev = nullptr, *next = nullptr;
    };

    class LRUCollection
    {
    public:
        LRUCollection(const PageIndexer& indexer);
        ~LRUCollection();

        bool RemoveBack();
        bool AddFront(const Page& page, int x, int y);

        CachePage* GetListEnd() { return m_list_end; }

        bool Find(const Page& page) const;

        void Touch(const Page& page);

        int Size() const { return m_map.size(); }

        void Clear();

    private:
        void Check();

    private:
        const PageIndexer& m_indexer;

        std::unordered_map<int, CachePage*> m_map;
        CachePage *m_list_begin, *m_list_end;

        CachePage* m_freelist;

    }; // LRUCollection

protected:
    PageLoader& m_loader;

    LRUCollection m_lru;

}; // PageCache

}