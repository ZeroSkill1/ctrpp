#include <ctrpp/types/tmd.hh>
#include <ctrpp/exceptions.hh>
#include <ctrpp/util/tools.hh>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace ctrpp::types::TMD;

int main(int argc, char *argv[])
{
	if (!(argc > 1))
	{
		std::cerr << "Invalid number of arguments\n";
	}

	TMD *t;

	try
	{
		t = new TMD(argv[1]);

		for (size_t i = 0; i < t->ContentChunkRecords.size(); i++)
		{
			printf("CONTENT CHUNK RECORD %04X.%08X:\n\nSize: %lu\nFlag: %04X\nHash: %s\n\n", t->ContentChunkRecords[i]->content_index(), t->ContentChunkRecords[i]->content_id(), t->ContentChunkRecords[i]->content_size(), t->ContentChunkRecords[i]->content_type(), ctrpp::util::buffer_to_hex(t->ContentChunkRecords[i]->raw_chunk_record_data->hash, HASH_SIZE, true).c_str());
		}
	}
	catch (ctrpp::exceptions::BaseException& ex)
	{
		std::cerr << "Caught exception! Message:\n" << ex.what() << "\n";
		return -1;
	}

	printf("%016lX\n", t->title_id());

	delete t;

	return 0;
}