//
// Created by marijn on 1/8/25.
//

#ifndef FLIB_FILE_HPP
#define FLIB_FILE_HPP

#include <fcntl.h>
#include <unistd.h>

#include "types.hpp"



/*<!
 * file_exception
 */
typedef enum {
	NOT_FOUND,
	MAGIC_MISMATCH
} file_exception_t;

template<file_exception_t T>
class file_exception : public std::exception {
private:
	str_t file = nullptr;
public:
	explicit file_exception(str_t file) { this->file = file; };

	// TODO: type switching
	_ND str_t what() const noexcept override;
};



/*<!
 * file
 */
typedef enum {
	READ_ONLY =		O_RDONLY,
	WRITE_ONLY =	O_WRONLY,
	READ_WRITE =	O_RDWR,
	APPEND =		O_APPEND,
	TRUNCATE =		O_TRUNC,

	CREATE =		O_CREAT,
	EXCLUSIVE =		O_EXCL,		// error if create and file exists

	ACCMODE =		O_ACCMODE,	// ?
	NO_CTTY =		O_NOCTTY,
	NON_BLOCK =		O_NONBLOCK,
	SYNC =			O_SYNC
} oflag_t;

#undef SEEK_END
typedef enum {
	SEEK_START =	0U,
	SEEK_CURRENT =	1U,
	SEEK_END =		2U
} sflag_t;


class file {
public:
	file(str_t fname, oflag_t flags, uint32_t mode = 0);
	~file() { ::close(this->fd); }

	// TODO: functions support 64 bit?
	_FI void read(void* const dst, uint64_t size, uint64_t offset) const	{ ::pread(this->fd, dst, size, offset); }
	_FI void read(void* const dst, uint64_t size)							{ ::read(this->fd, dst, size); }
	_FI void write(const void* const src, uint64_t size)					{ ::write(this->fd, src, size); }

	_FI uint64_t tell(void)	const											{ return ::lseek(this->fd, 0, SEEK_CUR); }
	_FI uint64_t seek(uint64_t offset, sflag_t mode = SEEK_CURRENT)			{ return ::lseek(this->fd, offset, mode); }

	// TODO: size? not set in this class!! (can it be disabled for base class only?)
	_ND _FI uint64_t size(void) const { return this->fsize; }
protected:
	str_t		fname = nullptr;
	int			fd =	-1;
	uint64_t	fsize =	-1;
};


#endif //FLIB_FILE_HPP
