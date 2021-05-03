#ifndef __exefs_hh
#define __exefs_hh

#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>

#include <vector>

#include <util.hh>
#include <ints.hh>

namespace ctrpp
{
	namespace types
	{
		namespace ExeFS
		{
			#define BUF_SIZE 16384

			static const u8 empty_exefs_entry[0x10] =
			{
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
			};

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

				bool success_parsed;

				u32 write_entry_to_buffer(u8 *buffer, const char *entry_name);
				u32 write_entry_to_file(const char *filename, const char *entry_name);

				bool verify();

				~ExeFS();
			private:
				FILE *exefs_f = nullptr;
			};
		}
	}
}

#endif