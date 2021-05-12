#include <ctrpp/util/tools.hh>

using namespace ctrpp::exceptions::crypto;
using namespace ctrpp::exceptions::fs;

std::string ctrpp::util::buffer_to_hex(u8 *data, size_t length, bool uppercase)
{
	const char *format = uppercase ? UPPER_HEX : LOWER_HEX;
	char tmp[3];
	std::string out;

	for (u32 i = 0; i < length; i++)
	{
		snprintf(tmp, 3, format, data[i]);

		out += std::string(tmp, 2);
	}

	return out;
}

u32 ctrpp::util::round_align_32(u32 in, u32 align)
{
	return (u32)std::ceil((double)in / align) * align;
}

u32 ctrpp::util::round_align_64(u64 in, u32 align)
{
	return (u32)std::ceil((double)in / align) * align;
}

void ctrpp::util::print_unicode(char16 *data, u32 length)
{
	std::u16string unc = std::u16string(data, length);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;

	std::cout << conv.to_bytes(unc) << "\n";
}
