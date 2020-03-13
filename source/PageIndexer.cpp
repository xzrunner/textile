#include "textile/PageIndexer.h"
#include "textile/VirtualTextureInfo.h"

#include <algorithm>

#include <assert.h>

namespace textile
{

PageIndexer::PageIndexer(const VirtualTextureInfo& info)
{
	m_mip_count = static_cast<int>(std::log2(std::min(info.PageTableWidth(), info.PageTableHeight()))) + 1;

	m_sizes.resize(m_mip_count);
	for (int i = 0; i < m_mip_count; ++i)
    {
		m_sizes[i].x = (info.vtex_width / info.tile_size) >> i;
        m_sizes[i].y = (info.vtex_height / info.tile_size) >> i;
	}

	m_offsets.resize(m_mip_count);
	m_page_count = 0;
	for (int i = 0; i < m_mip_count; ++i) {
		m_offsets[i] = m_page_count;
		m_page_count += m_sizes[i].x * m_sizes[i].y;
	}

	m_pages.resize(m_page_count);
	for (int i = 0; i < m_mip_count; ++i)
	{
		for (int y = 0; y < m_sizes[i].y; ++y) {
			for (int x = 0; x < m_sizes[i].x; ++x) {
				Page p(x, y, i);
				m_pages[CalcPageIdx(p)] = p;
			}
		}
	}
}

int PageIndexer::CalcPageIdx(const Page& page) const
{
	assert(page.mip >= 0 && page.mip < m_mip_count);
	int offset = m_offsets[page.mip];
	int stride = m_sizes[page.mip].x;
	return offset + page.y * stride + page.x;
}

const Page& PageIndexer::QueryPageByIdx(int idx) const
{
	assert(idx >= 0 && idx < static_cast<int>(m_pages.size()));
	return m_pages[idx];
}

}