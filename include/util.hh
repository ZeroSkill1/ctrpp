#ifndef __util_hh
#define __util_hh

#include <openssl/sha.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <codecvt>
#include <string>
#include <locale>
#include <cmath>

#include <ints.hh>

#define UPPER_HEX "%02X"
#define LOWER_HEX "%02x"
#define BUF_SIZE 16384

namespace ctrpp
{
	namespace util
	{
		std::string buffer_to_hex(u8 *data, u32 length, bool uppercase = false);
		u32 round_align_32(u32 in, u32 align);
		u32 round_align_64(u64 in, u32 align);
		void print_unicode(char16 *data, u32 length);
		long check_file(const char *filename);

		namespace hash
		{
			namespace sha256
			{
				int hash_file_part(FILE *f, u8 *out_hash, u32 offset, u32 size);
			}
		}

		namespace io
		{
			int copy_file_part_buffered(FILE *in, FILE *out, u32 in_offset, u32 size, u32 out_offset, bool close_in = false, bool close_out = false);
			int copy_file_part_buffered(FILE *in, const char *out, u32 offset, u32 size, bool close_in = false, bool close_out = false);
		}
	}
}

#endif