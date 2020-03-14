#pragma once

#include "textile/VTexInfo.h"

#include <string>
#include <fstream>
#include <mutex>

namespace textile
{

class TileDataFile : private boost::noncopyable
{
public:
	TileDataFile(const std::string& filepath);
	~TileDataFile();

	void ReadPage(int index, uint8_t* data) const;
	void WritePage(int index, const uint8_t* data);

	const VTexInfo& GetVTexInfo() const { return m_vtex_info; }

    auto GetTileSize() const { return m_tile_size; }

    static void ReadHeader(VTexInfo& dst, std::fstream& src);
    static void WriteHeader(std::fstream& dst, const VTexInfo& src);

private:
	static const int DATA_OFFSET = 16;

private:
	mutable VTexInfo m_vtex_info;

	mutable size_t m_tile_size;

	mutable std::fstream m_file;

	mutable std::mutex m_mutex;

}; // TileDataFile

}