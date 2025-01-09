#include <stdio.h>
#include <stdlib.h>

#include "BMP.hpp"


void test_txt(void) {
	file f("../test/files/text.txt", READ_ONLY);

	uint64_t fsize = f.size();
	char* buf = (char*)malloc(fsize + 1);
	f.read(buf, fsize);
	buf[fsize] = 0;

	printf("flib: read %lu chars -> %s\n", fsize, buf);
}

void test_bmp(void) {
	BMP bmp("../test/files/image.bmp", READ_ONLY);

	uint64_t fsize = bmp.size();

	printf("flib: bmp size: %lu\n", fsize);
}


int main() {
	test_bmp();
	return 0;
}
