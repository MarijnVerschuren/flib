//
// Created by marijn on 1/9/25.
//

#ifndef FLIB_BMP_HPP
#define FLIB_BMP_HPP

#include "file.hpp"
#include "serializer.hpp"
#include "image.hpp"

#define BMP_MAGIC 0x4D42U
#define BMP_DIB_SIZE 40

#define BMP_FILE_SIZE_POS 0x02U
#define BMP_DATA_OFFSET_POS 0x0AU
#define BMP_DIB_HEADER_END 0x36U


typedef enum {
	BMP_PIXEL_1_BIT =	1U,		// 1 bit monochrome bitmap
	BMP_PIXEL_4_BIT =	4U,		// 4 bit palletized pixel data
	BMP_PIXEL_8_BIT =	8U,		// 8 bit palletized pixel data
	BMP_PIXEL_16_BIT =	16U,	// 16 bit RGB
	BMP_PIXEL_24_BIT =	24U,	// 24 bit RGB
	// TODO customs
	BMP_PIXEL_32_BIT =	32U,	// 32 bit RGBA
} BMP_pixel_format_t;


class BMP : public serializer {
public:
	void load(file_t file) override;
	void save(file_t file) override;

	operator image_t(void);

private:
	struct {
		uint32_t width;
		uint32_t height;
		uint16_t planes;
		uint16_t bits_per_pixel;	// BMP_pixel_format_t
		uint32_t compression;		// BMP_compression_t
		uint32_t image_size;
		uint32_t pixel_per_meter_x;
		uint32_t pixel_per_meter_y;
		uint32_t color_count;
		uint32_t important_color_count;
	} info_header {};

	uint32_t*	palette = nullptr;
	void*		pixel_data = nullptr;
};


#endif //FLIB_BMP_HPP
