#ifndef __util_hh
#define __util_hh

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <codecvt>
#include <string>
#include <locale>
#include <cmath>

#include <ctrpp/ints.hh>

namespace ctrpp
{
	namespace util
	{
		std::string buffer_to_hex(u8 *data, u32 length, bool uppercase = false);
		u32 round_align_32(u32 in, u32 align);
		u32 round_align_64(u64 in, u32 align);
		void print_unicode(char16 *data, u32 length);

		namespace crypto
		{
			int hash_buffer(u8 *buf, u64 size, u8 *out_hash);
			int hash_file(FILE *f, u64 size, u8 *out_hash);
			int hash_file_part(FILE *f, u8 *out_hash, u64 offset, u64 size);
			int hash_file_part_read(FILE *f, u8 *out_hash, u8 *out, u64 offset, u64 size);
		}

		namespace io
		{
			long check_file(const char *filename);
			int copy_file_part_buffered(FILE *in, FILE *out, u64 in_offset, u64 size, u64 out_offset, bool close_in = false, bool close_out = false);
			int copy_file_part_buffered(FILE *in, const char *out, u64 offset, u64 size, bool close_in = false, bool close_out = false);
		}
	}
}

#endif