//
// Created by marijn on 1/9/25.
//

#ifndef FLIB_BMP_HPP
#define FLIB_BMP_HPP
#include "file.hpp"

#define BMP_MAGIC 0x4D42U
#define BMP_DIB_SIZE 40



class BMP : public file {
public:
	BMP(str_t fname, uint32_t mode = 666);

	void open(oflag_t flags);

private:
	struct {
		uint32_t width;
		uint32_t height;
		uint16_t planes;
		uint16_t bits_per_pixel;
		uint32_t compression;
		uint32_t image_size;
		uint32_t pixel_per_meter_x;
		uint32_t pixel_per_meter_y;
		uint32_t color_count;
		uint32_t important_color_count;
	} info_header;
};


#endif //FLIB_BMP_HPP
