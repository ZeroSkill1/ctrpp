#ifndef __util_hh
#define __util_hh

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

namespace ctrpp
{
	namespace util
	{
		std::string buffer_to_hex(u8 *data, u32 length, bool uppercase = false);
		u32 round_align_32(u32 in, u32 align);
		u32 round_align_64(u64 in, u32 align);
		void print_unicode(char16 *data, u32 length);
		long check_file(const char *filename);
	}
}

#endif