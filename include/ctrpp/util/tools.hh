#ifndef __tools_hh
#define __tools_hh

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <codecvt>
#include <string>
#include <locale>
#include <cmath>

#include <ctrpp/exceptions.hh>
#include <ctrpp/ints.hh>

namespace ctrpp
{
	namespace util
	{
		std::string buffer_to_hex(u8 *data, size_t length, bool uppercase = false);
		u32 round_align_32(u32 in, u32 align);
		u32 round_align_64(u64 in, u32 align);
		void print_unicode(char16 *data, u32 length);
	}
}

#endif