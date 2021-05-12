#ifndef __exefs_hh
#define __exefs_hh

#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>

#include <vector>

#include <ctrpp/exceptions.hh>
#include <ctrpp/util/tools.hh>
#include <ctrpp/util/io.hh>
#include <ctrpp/ints.hh>

namespace ctrpp
{
	namespace types
	{
		namespace ExeFS
		{
			static const u8 empty_exefs_entry[0x10] =
				{
					0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00};

			typedef struct __attribute__((packed)) exefs_file_entry
			{
				char file_name[8];
				u32 file_offset;
				u32 file_size;
			} exefs_file_entry;

			typedef struct __attribute__((packed)) exefs_header
			{
				exefs_file_entry file_entries[10];
				u8 reserved[0x20];
				u8 file_hashes[10][0x20];
			} exefs_header;

			class ExeFS
			{
			public:
				ExeFS();
				ExeFS(const char *filename);

				std::vector<exefs_file_entry *> file_entries;

				exefs_header *header = nullptr;

				void export_entry(const char *filename, const char *entry_name);
				bool verify();

				~ExeFS();

			private:
				FILE *exefs_f = nullptr;
			};
		}
	}
}

#endif