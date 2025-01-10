//
// Created by marijn on 1/8/25.
//

#include "file.hpp"



/*<!
 * file_exception
 */
template<> str_t file_exception<NOT_FOUND>::what() const noexcept			{ return file; }
template<> str_t file_exception<MAGIC_MISMATCH>::what() const noexcept		{ return file; }



/*<!
 * file
 */
file::file(str_t fname, uint32_t permission) {
	this->fname = fname;  // TODO: open seperate
	this->permission = permission;
}

void file::open(oflag_t flags) {
	this->fd = ::open(this->fname, flags, this->permission);
	if (fd < 0) { throw file_exception<NOT_FOUND>(fname); }

}

void file::close(void) {
	if (this->fd < 0) { return; }
	::close(this->fd); this->fd = -1;
}

uint64_t file::size(void) const {
	uint64_t position = tell();
	::lseek(this->fd, 0, SEEK_END);
	uint64_t size = tell();
	::lseek(this->fd, position, SEEK_START);
	return size;
}
uint64_t file::size(void) {
	seek(0, SEEK_END);
	return tell();
}
