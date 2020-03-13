#pragma once

#include <boost/noncopyable.hpp>

namespace textile
{

struct VirtualTextureInfo
{
    size_t vtex_width  = 0;
    size_t vtex_height = 0;

    size_t tile_size   = 0;
    size_t border_size = 0;

    size_t channels = 0;
    size_t bytes    = 0;

	//VirtualTextureInfo() {}
	//VirtualTextureInfo(const VirtualTextureInfo& info)
	//	: virtual_texture_size(info.virtual_texture_size), tile_size(info.tile_size), border_size(info.border_size) {}
	//VirtualTextureInfo(int virtual_texture_size, int tile_size, int border_size)
	//	: virtual_texture_size(virtual_texture_size), tile_size(tile_size), border_size(border_size) {}

	int PageSize() const { return tile_size + 2 * border_size; }
	int PageTableWidth() const { return vtex_width / tile_size; }
    int PageTableHeight() const { return vtex_height / tile_size; }

}; // VirtualTextureInfo

}