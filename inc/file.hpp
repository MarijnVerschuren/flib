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
	MAGIC_MISMATCH,
	FILE_CORRUPTED
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
	file(str_t fname, uint32_t permission = 666);

	virtual void open(oflag_t flags);
	void close(void);

	_ND uint32_t size(void) const;
	_ND _FI uint32_t size(void)												{ return ::lseek(this->fd, 0, SEEK_END); }

	_ND _FI uint32_t tell(void)	const										{ return ::lseek(this->fd, 0, SEEK_CUR); }
	_FI uint32_t seek(uint32_t offset, sflag_t mode = SEEK_CURRENT)			{ return ::lseek(this->fd, offset, mode); }

	_FI void read(void* const dst, uint32_t size, uint32_t offset) const	{ ::pread(this->fd, dst, size, offset); }
	_FI void read(void* const dst, uint32_t size)							{ ::read(this->fd, dst, size); }
	_ND _FI uint8_t read_u8(void)											{ uint8_t data; ::read(this->fd, &data, 1); return data; }
	_ND _FI uint8_t read_u8(uint32_t offset) const							{ uint8_t data; ::pread(this->fd, &data, 1, offset); return data; }
	_ND _FI uint16_t read_u16(void)											{ uint16_t data; ::read(this->fd, &data, 2); return data; }
	_ND _FI uint16_t read_u16(uint32_t offset) const						{ uint16_t data; ::pread(this->fd, &data, 2, offset); return data; }
	_ND _FI uint32_t read_u32(void)											{ uint32_t data; ::read(this->fd, &data, 4); return data; }
	_ND _FI uint32_t read_u32(uint32_t offset) const						{ uint32_t data; ::pread(this->fd, &data, 4, offset); return data; }
	_ND _FI uint64_t read_u64(void)											{ uint64_t data; ::read(this->fd, &data, 8); return data; }
	_ND _FI uint64_t read_u64(uint32_t offset) const						{ uint64_t data; ::pread(this->fd, &data, 8, offset); return data; }

	_FI void write(const void* const src, uint32_t size)					{ ::write(this->fd, src, size); }

protected:
	str_t		fname = nullptr;
	uint32_t	permission = 0;

	int			fd =	-1;
};


#endif //FLIB_FILE_HPP
