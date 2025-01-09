//
// Created by marijn on 1/9/25.
//

#include "BMP.hpp"

BMP::BMP(str_t fname, oflag_t flags, uint32_t mode) : file(fname, flags, mode) {
	uint16_t signature;
	read(&signature, 2);
	if (signature != BMP_MAGIC) { throw file_exception<MAGIC_MISMATCH>(fname); }
	uint32_t file_size;
	read(&file_size, 4);
}
