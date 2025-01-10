//
// Created by marijn on 1/9/25.
//

#include "BMP.hpp"


BMP::BMP(str_t fname, uint32_t mode) : file(fname, mode) {}

void BMP::open(oflag_t flags) {
	file::open(flags);
	uint64_t fsize = size();
	if (fsize == 0) { return; }  // empty / new file

	uint16_t signature;
	read(&signature, 2);

	if (signature != BMP_MAGIC) { throw file_exception<MAGIC_MISMATCH>(fname); }
	uint32_t file_size;
	read(&file_size, 4);
}