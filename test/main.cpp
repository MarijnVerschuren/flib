#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BMP.hpp"


void test_txt(void) {
	file_t f("../test/files/text.txt");

	uint64_t fsize = f.size();
	char* buf = (char*)malloc(fsize + 1);
	f.read(buf, fsize);
	buf[fsize] = 0;

	printf("flib: read %lu chars -> %s\n", fsize, buf);
	f.close();
}


uint32_t pix_transform_not(uint32_t pix, uint8_t) {
	return ~pix;
}
uint32_t pix_transform(uint32_t pix, uint8_t) {
	uint8_t r = ((pix >> 16U) & 0xFFU);
	uint8_t g = ((pix >> 8U) & 0xFFU);
	uint8_t b = ((pix >> 0U) & 0xFFU);
	return ((r << 16U) | (g << 8U) | (b << 0U));
}

uint32_t pix_kernel_avg(void** const kernel, uint8_t* bit_offs, uint8_t kw, uint8_t kh, uint8_t bpp) {
	uint32_t r_acc = 0;
	uint32_t g_acc = 0;
	uint32_t b_acc = 0;
	uint16_t tmp, *ptr;
	for (uint8_t y = 0; y < kh; y++) {
		ptr = (uint16_t*)kernel[y];
		for (uint8_t x = 0; x < kw; x++) {
			tmp = ptr[x];
			r_acc += ((tmp >> 10U) & 0x1FU);
			g_acc += ((tmp >> 5U) & 0x1FU);
			b_acc += ((tmp >> 0U) & 0x1FU);
		}
	}
	double elems = kw * kh;
	uint8_t r_avg = (uint8_t)(((double)r_acc) / elems);
	uint8_t g_avg = (uint8_t)(((double)g_acc) / elems);
	uint8_t b_avg = (uint8_t)(((double)b_acc) / elems);
	return (((r_avg & 0x1FU) << 10U) | ((g_avg & 0x1FU) << 5U) | ((b_avg & 0x1FU) << 0U));
}


void test_bmp(void) {
	BMP bmp;
	bmp.load("../test/files/image1.bmp");

	image_t img = bmp;
	//img.map(pix_transform_not);
	//img.map(pix_transform);
	img.map(pix_kernel_avg, 10, 1);

	bmp.save("../test/files/image_edit.bmp");
}


int main() {
	test_bmp();
	return 0;
}
