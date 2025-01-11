//
// Created by marijn on 1/9/25.
//

#include "BMP.hpp"


BMP::BMP(str_t fname, uint32_t mode) : file(fname, mode) {}

void BMP::open(oflag_t flags) {
	file::open(flags);
	uint64_t fsize = size();
	if (fsize == 0) { return; }  // empty / new file

	seek(0, SEEK_START);
	uint16_t signature = read_u16();
	if (signature != BMP_MAGIC) { throw file_exception<MAGIC_MISMATCH>(fname); }
	uint32_t file_size =			read_u32();
	seek(4);  // skip reserved bytes
	uint32_t pixel_array_offset =	read_u32();
	uint32_t dib_header_size =		read_u32();
	if (dib_header_size != BMP_DIB_SIZE) { throw file_exception<FILE_CORRUPTED>(fname); }
	read(&info_header, BMP_DIB_SIZE - 4);
	return;
}