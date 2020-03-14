#pragma once

#include "textile/Page.h"

#include <SM_Vector.h>

#include <boost/noncopyable.hpp>

#include <vector>

namespace textile
{

struct VTexInfo;

class PageIndexer : boost::noncopyable
{
public:
	PageIndexer(const VTexInfo& info);

	int CalcPageIdx(const Page& page) const;
	const Page& QueryPageByIdx(int idx) const;

	int GetPageCount() const { return m_page_count; }

private:
	int m_page_count;
	int m_mip_count;

	std::vector<int> m_offsets;
    std::vector<sm::ivec2> m_sizes;

	std::vector<Page> m_pages;

}; // PageIndexer

}