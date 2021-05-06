#include <ctrpp/crypto.hh>
#include <ctrpp/util.hh>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "no arguments provided!\n";
		return -1;
	}

	std::cout << ctrpp::crypto::CTR_CONSTANT << "\n";
	std::cout << ctrpp::crypto::TWL_CONSTANT << "\n";

	ctrpp::crypto::Engine engine = ctrpp::crypto::Engine(argv[1]);
	ctrpp::crypto::print_error(engine.result);

	if (engine.result != 0)
	{
		return -1;
	}

	u8 key_y[0x10] = { 0xB5, 0xF2, 0xC3, 0x2C, 0x7C, 0x6E, 0xF8, 0x0F, 0xBA, 0xB9, 0x25, 0xAA, 0x16, 0x3F, 0x7C, 0xA3 };
	uint128_t key_y_int = ctrpp::ints::to_be_u128(key_y);

	u8 key_x[0x10] = { 0xB9, 0x8E, 0x95, 0xCE, 0xCA, 0x3E, 0x4D, 0x17, 0x1F, 0x76, 0xA9, 0x4D, 0xE9, 0x34, 0xC0, 0x53 };
	uint128_t key_x_int = ctrpp::ints::to_be_u128(key_x);

	std::cout << "KeyX: " << ctrpp::util::buffer_to_hex(key_x, KEY_SIZE, true) << "\n";
	std::cout << "KeyY: " << ctrpp::util::buffer_to_hex(key_y, KEY_SIZE, true) << "\n";

	engine.ctr_keygen(0x2C, key_x_int, key_y_int);

	u8* whatever = engine.normal_key[0x2C];

	std::cout << "NormalKey Slot 0x2C: " << ctrpp::util::buffer_to_hex(whatever, KEY_SIZE, true) << "\n";

	perror(strerror(errno));

	return 0;
}