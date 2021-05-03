#include <types/exefs.hh>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <util.hh>

using namespace ctrpp::types::ExeFS;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "no arguments provided!\n";
		return -1;
	}

	ExeFS exefs = ExeFS(argv[1]);

	if (exefs.success_parsed)
	{
		std::cout << "ok!\n";
		
		for (u32 i = 0; i < exefs.file_entries.size(); i++)
		{
			std::cout << "FILE ENTRY:\n\n";
			std::cout << "Name: " << exefs.file_entries[i]->file_name << "\n";
			std::cout << "Offset: 0x" << std::hex << exefs.file_entries[i]->file_offset << "\n";
			std::cout << "Size: 0x" << std::hex << exefs.file_entries[i]->file_size << "\n";
			std::cout << "\n===============================================================\n\n";
		}
	}
	else
	{
		std::cout << "not ok!\n";
	}

	perror(strerror(errno));

	return 0;
}