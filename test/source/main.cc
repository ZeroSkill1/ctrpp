#include <types/smdh.hh>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <util.hh>

using namespace ctrpp;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "no arguments provided!" << std::endl;
		return -1;
	}

	ctrpp::types::SMDH::SMDH smdh(argv[1]);

	if (smdh.success_parsed)
	{
		std::cout << "ok!" << std::endl;
		
		ctrpp::util::print_unicode(smdh.raw_smdh_data->title_names[1].short_desc, 0x40);

		char hex[9];

		snprintf(hex, 9, "%08x", smdh.region_lockout());

		std::cout << "flag: 0x" << hex << std::endl;
	}
	else
	{
		std::cout << "not ok!" << std::endl;
	}

	perror(strerror(errno));

	return 0;
}