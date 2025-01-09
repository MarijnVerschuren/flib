//
// Created by marijn on 1/9/25.
//

#ifndef FLIB_BMP_HPP
#define FLIB_BMP_HPP
#include "file.hpp"

#define BMP_MAGIC 0x4D42U


class BMP : public file {
public:
	BMP(str_t fname, oflag_t flags, uint32_t mode = 0);

	_ND _FI uint64_t size(void) const { return this->fsize; }
private:
	uint64_t	fsize =	-1;
};


#endif //FLIB_BMP_HPP
