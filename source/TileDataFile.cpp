#include "textile/TileDataFile.h"

namespace textile
{

TileDataFile::TileDataFile(const std::string& filepath)
{
	m_file.open(filepath.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::app);

	ReadHeader(m_vtex_info, m_file);
    m_tile_size = m_vtex_info.PageSize() * m_vtex_info.PageSize() * m_vtex_info.channels * m_vtex_info.bytes;
}

TileDataFile::~TileDataFile()
{
	m_file.close();
}

void TileDataFile::ReadPage(int index, uint8_t* data) const
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_file.seekg(DATA_OFFSET + static_cast<uint64_t>(m_tile_size) * index);
	m_file.read(reinterpret_cast<char*>(data), m_tile_size);
}

void TileDataFile::WritePage(int index, const uint8_t* data)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_file.seekp(DATA_OFFSET + static_cast<uint64_t>(m_tile_size) * index);
	m_file.write(reinterpret_cast<const char*>(data), m_tile_size);
}

void TileDataFile::ReadHeader(VTexInfo& dst, std::fstream& src)
{
    src.seekg(0, std::ios_base::beg);

    uint32_t w = 0;
    src.read(reinterpret_cast<char*>(&w), sizeof(w));
    dst.vtex_width = w;

    uint32_t h = 0;
    src.read(reinterpret_cast<char*>(&h), sizeof(h));
    dst.vtex_height = h;

    uint16_t tile_sz = 0;
    src.read(reinterpret_cast<char*>(&tile_sz), sizeof(tile_sz));
    dst.tile_size = tile_sz;

    uint16_t border_sz = 0;
    src.read(reinterpret_cast<char*>(&border_sz), sizeof(border_sz));
    dst.border_size = border_sz;

    uint16_t channels = 0;
    src.read(reinterpret_cast<char*>(&channels), sizeof(channels));
    dst.channels = channels;

    uint16_t bytes = 0;
    src.read(reinterpret_cast<char*>(&bytes), sizeof(bytes));
    dst.bytes = bytes;
}

void TileDataFile::WriteHeader(std::fstream& dst, const VTexInfo& src)
{
    dst.seekp(0, std::ios_base::beg);

    // header 16 byte
    const uint32_t w = src.vtex_width;
    dst.write(reinterpret_cast<const char*>(&w), sizeof(w));
    const uint32_t h = src.vtex_height;
    dst.write(reinterpret_cast<const char*>(&h), sizeof(h));
    const uint16_t tile_sz = static_cast<uint16_t>(src.tile_size);
    dst.write(reinterpret_cast<const char*>(&tile_sz), sizeof(tile_sz));
    const uint16_t border_sz = static_cast<uint16_t>(src.border_size);
    dst.write(reinterpret_cast<const char*>(&border_sz), sizeof(border_sz));
    const uint16_t channels = static_cast<uint16_t>(src.channels);
    dst.write(reinterpret_cast<const char*>(&channels), sizeof(channels));
    const uint16_t bytes = static_cast<uint16_t>(src.bytes);
    dst.write(reinterpret_cast<const char*>(&bytes), sizeof(bytes));
}

}