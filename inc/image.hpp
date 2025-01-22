//
// Created by marijn on 1/13/25.
//

#ifndef FLIB_IMAGE_HPP
#define FLIB_IMAGE_HPP
#include <malloc.h>

#include "types.hpp"


typedef uint32_t (*pixel_fn)(uint32_t pixel, uint8_t bpp);
typedef uint32_t (*kernel_fn)(void** const kernel, uint8_t* bit_offsets, uint8_t kernel_width, uint8_t kernel_height, uint8_t bpp);

class image_t {
public:
	// pixel_data, bits_per_pixel, palette, bits_per_entry, width, height
	image_t(void* pix_data, uint8_t bpp, void* palette, uint8_t bpe, uint32_t width, uint32_t height);
	image_t(void* pix_data, uint8_t bpp, uint32_t width, uint32_t height);

	_ND _FI uint32_t get_width(void) const	{ return this->width; }
	_ND _FI uint32_t get_height(void) const	{ return this->height; }

	_ND uint32_t get(uint32_t x, uint32_t y) const;
	void set(uint32_t x, uint32_t y, uint32_t val);

	void map(pixel_fn fn);
	void map(kernel_fn fn, uint8_t kernel_width, uint8_t kernel_height);

protected:
	uint8_t		bpp = 0, bpi = 0;
	uint32_t	width = 0, height = 0;
	void*		palette = nullptr;
	void*		pixel_data = nullptr;

	uint32_t	size, mask;
};


#endif //FLIB_IMAGE_HPP
