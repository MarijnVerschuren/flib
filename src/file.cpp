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
file::file(str_t fname, oflag_t flags, uint32_t mode) {
	this->fname = fname;
	this->fd = open(fname, flags, mode);
	if (fd < 0) { throw file_exception<NOT_FOUND>(fname); }
}
