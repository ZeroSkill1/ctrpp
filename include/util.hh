#ifndef __util_hh
#define __util_hh

#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ints.hh>
#include <codecvt>
#include <string>
#include <locale>

#define UPPER_HEX "%02X"
#define LOWER_HEX "%02x"

namespace ctrpp
{
	namespace util
	{
		std::string buffer_to_hex(u8 *data, u32 length, bool uppercase = false);
		void print_unicode(char16 *data, u32 length);
	}
}

#endif