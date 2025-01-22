//
// Created by marijn on 1/9/25.
//
#include "BMP.hpp"


void BMP::load(file_t file) {
	bool closed = !file.is_open();
	if (closed) { file.open(READ_ONLY); }

	uint64_t fsize = file.size();
	if (fsize == 0) { return; }  // empty / new file

	// check signature
	file.seek(0, SEEK_START);
	uint16_t signature = file.read_u16();
	if (signature != BMP_MAGIC) { throw file_exception<MAGIC_MISMATCH>(file.name()); }

	// load header data
	file.seek(8);  // skip file size and reserved bytes
	uint32_t pixel_array_offset =	file.read_u32();
	uint32_t dib_header_size =		file.read_u32();
	// TODO: larger dibs<<<
	if (dib_header_size != BMP_DIB_SIZE) { throw file_exception<FILE_CORRUPTED>(file.name()); }
	file.read(&info_header, BMP_DIB_SIZE - 4);

	// load color palette if any
	if (this->info_header.color_count) {
		if (this->palette) { free(this->palette); }
		this->palette = (uint32_t*)malloc(4 * this->info_header.color_count);
		if (!this->palette) { throw memory_exception("BMP::load_color_palette(void)"); }
		file.read(this->palette, 4 * this->info_header.color_count);
	}

	// load pixel data
	if (!this->info_header.image_size) {
		this->info_header.image_size =
			this->info_header.width * this->info_header.height *
			(1 + this->info_header.bits_per_pixel / 8);
	}
	if (this->pixel_data) { free(this->pixel_data); }
	this->pixel_data = malloc(this->info_header.image_size);
	if (!this->pixel_data) { throw memory_exception("BMP::open(oflag_t flags)"); }
	file.read(this->pixel_data, this->info_header.image_size, pixel_array_offset);

	if (closed) { file.close(); }
}


void BMP::save(file_t file) {
	bool closed = !file.is_open();
	if (closed) { file.open(WRITE_ONLY | CREATE); }

	file.seek(0, SEEK_START);
	file.write_u16(BMP_MAGIC);													// write magic signature
	file.write_u32(0, 0x06U);													// write reserved field
	file.write_u32(BMP_DIB_SIZE, 0x0EU);										// write DIB header size
	file.write(&this->info_header, BMP_DIB_SIZE - 4, 0x12U);					// write header
	file.seek(BMP_DIB_HEADER_END);												// seek the position after header
	if (this->info_header.color_count) {
		if (!this->palette) { throw memory_exception("BMP::save(void)"); }
		file.write(this->palette, 4 * this->info_header.color_count);		// write color palette
	}
	file.write_u32(file.tell(), BMP_DATA_OFFSET_POS);							// write pixel data offset
	if (!this->pixel_data) { throw memory_exception("BMP::save(void)"); }
	file.write(this->pixel_data, this->info_header.image_size);					// write pixel data
	file.write_u32(file.tell(), BMP_FILE_SIZE_POS);								// write file size

	if (closed) { file.close(); }
}

BMP::operator image_t(void) {
	return image_t(
		pixel_data, this->info_header.bits_per_pixel,
		palette, 24,
		this->info_header.width,
		this->info_header.height
	);
}