//
// Created by marijn on 1/13/25.
//
#include "image.hpp"


image_t::image_t(void* pix_data, uint8_t bpp, void* palette, uint8_t bpi, uint32_t width, uint32_t height) {
	this->pixel_data = pix_data;
	this->palette = palette;
	this->bpp = bpp;
	this->bpi = bpi;
	this->width = width;
	this->height = height;

	size = (width * height * bpp);
	size = (size >> 3U) + ((size & 0x7U) > 0);
	mask = (0b1U << bpp) - 1;
}

image_t::image_t(void* pix_data, uint8_t bpp, uint32_t width, uint32_t height) : image_t(pix_data, bpp, nullptr, 0, width, height) {}

uint32_t image_t::get(uint32_t x, uint32_t y) const {
	if (x > width || y > height) { return 0; }  // TODO: error
	uint32_t bit_off = (x + (y * width)) * bpp;
	uint32_t byte_off = bit_off >> 3U;
	bit_off &= 0x7U;
 	return (*((uint32_t*)(&((uint8_t*)pixel_data)[byte_off])) >> bit_off) & mask;
}

void image_t::set(uint32_t x, uint32_t y, uint32_t val) {
	if (x > width || y > height) { return; }  // TODO: error
	uint32_t bit_off = (x + (y * width)) * bpp;
	uint32_t byte_off = bit_off >> 3U;
	bit_off &= 0x7U;
	uint32_t* ptr = ((uint32_t*)(&((uint8_t*)pixel_data)[byte_off]));
	*ptr &= ~(mask << bit_off);
	*ptr |= (val & mask) << bit_off;
}

void image_t::map(pixel_fn fn) {
	uint8_t byte_per_pixel = bpp >> 3U;
	uint8_t bit_off = bpp & 0x7U;
	uint32_t tmp, *ptr;
	for (uint32_t i = 0; i < size; i += byte_per_pixel) {
		ptr = ((uint32_t*)(&((uint8_t*)pixel_data)[i]));
		tmp = (*ptr >> bit_off) & mask;
		*ptr &= ~(mask << bit_off);
		*ptr |= (fn(tmp, bpp) & mask) << bit_off;
	}
}

void image_t::map(kernel_fn fn, uint8_t kernel_width, uint8_t kernel_height) {
	if (kernel_width * kernel_height == 0) { return; }
	uint8_t byte_per_pixel = bpp >> 3U;
	uint32_t bit_off, tmp, *ptr;
	uint8_t kw, kh;
	void** ktmp = (void**)malloc((sizeof(void*) * kernel_height));
	uint8_t* otmp = (uint8_t*)malloc(kernel_height);
	for (uint32_t y = 0; y < height; y += kernel_height) {
		for (uint32_t x = 0; x < width; x += kernel_width) {
			kw = kernel_width < (width - x) ? kernel_width : (width - x);
			kh = kernel_height < (height - y) ? kernel_height : (height - y);
			for (uint8_t i = 0; i < kh; i++) {
				if (i > (height - y)) { break; }
				ktmp[i] = (void*)(&((uint8_t*)pixel_data)[(x + (y + i) * width) * byte_per_pixel]);
				otmp[i] = ((x + (y + i) * width) * bpp) & 0x7U;
			}
			tmp = fn(ktmp, otmp, kw, kh, bpp);
			for (uint8_t yk = 0; yk < kh; yk++) {
				for (uint8_t xk = 0; xk < kw; xk++) {
					bit_off = ((x + xk) + (y + yk) * width) * bpp;
					ptr = (uint32_t*)(&((uint8_t*)pixel_data)[bit_off >> 3U]);
					bit_off &= 0x7U;
					*ptr &= ~(mask << bit_off);
					*ptr |= (tmp << bit_off);
				}
			}
		}
	}
	free(ktmp); free(otmp);
}
