//
// Created by marijn on 1/8/25.
//

#include "file.hpp"



/*<!
 * file_exception
 */
template<> str_t file_exception<NOT_FOUND>::what() const noexcept			{ return file; }
template<> str_t file_exception<MAGIC_MISMATCH>::what() const noexcept		{ return file; }
template<> str_t file_exception<FILE_CORRUPTED>::what() const noexcept		{ return file; }



/*<!
 * file
 */
file_t::file_t(str_t fname, uint32_t permission) {
	this->fname = fname;  // TODO: open seperate
	this->permission = permission;
}

void file_t::open(oflag_t flags) {
	this->fd = ::open(this->fname, flags, this->permission);
	if (fd < 0) { throw file_exception<NOT_FOUND>(fname); }

}

void file_t::close(void) {
	if (this->fd < 0) { return; }
	::close(this->fd); this->fd = -1;
}

uint32_t file_t::size(void) const {
	uint32_t position = tell();
	::lseek(this->fd, 0, SEEK_END);
	uint32_t size = tell();
	::lseek(this->fd, position, SEEK_START);
	return size;
}

