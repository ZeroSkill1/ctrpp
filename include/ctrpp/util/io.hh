#ifndef __io_hh
#define __io_hh

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include <ctrpp/exceptions.hh>
#include <ctrpp/ints.hh>

namespace ctrpp
{
	namespace util
	{
		namespace crypto
		{
			void hash_buffer(u8 *buf, size_t size, u8 *out_hash);
			void hash_file(FILE *f, size_t size, u8 *out_hash);
			void hash_file_part(FILE *f, u8 *out_hash, off_t offset, size_t size);
			void hash_file_part_read(FILE *f, u8 *out_hash, u8 *out, off_t offset, size_t size);
		}

		namespace io
		{
			FILE *open_file_nsz(const char *filename, const char *mode);
			FILE *open_file(const char *filename, const char *mode, long *out_size);
			void copy_file_part_buffered(FILE *in, FILE *out, off_t in_offset, size_t size, off_t out_offset, bool close_in = false, bool close_out = true);
			void copy_file_part_buffered(FILE *in, const char *out, off_t offset, size_t size, bool close_in = false, bool close_out = true);
		}
	}
}

#endif