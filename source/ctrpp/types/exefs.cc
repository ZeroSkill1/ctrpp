#include <ctrpp/types/exefs.hh>

using namespace ctrpp::exceptions::types::exefs;
using namespace ctrpp::exceptions::fs;
using namespace ctrpp::types::ExeFS;
using namespace ctrpp::util::crypto;
using namespace ctrpp::util::io;
using namespace ctrpp::util;

ExeFS::ExeFS()
{
}

ExeFS::ExeFS(const char *filename)
{
	FILE *exefs = nullptr;
	long siz = 0;
	long exefs_fsizes = 0;

	exefs = open_file(filename, "r", &siz);

	this->exefs_f = exefs;
	this->header = new exefs_header;

	if (fread(this->header, 1, sizeof(exefs_header), this->exefs_f) != sizeof(exefs_header))
	{
		fclose(exefs);

		delete this->header;

		throw FileReadException(filename);
	}

	this->file_entries = std::vector<exefs_file_entry *>();

	for (u32 i = 0; i < 10; i++)
	{
		if (memcmp((&(this->header->file_entries[i])), empty_exefs_entry, 0x10) != 0)
		{
			exefs_fsizes += round_align_32(this->header->file_entries[i].file_size, 0x200);

			exefs_file_entry *et = new exefs_file_entry;
			memcpy(et, (&(this->header->file_entries[i])), sizeof(exefs_file_entry));

			this->file_entries.push_back(et);
		}
	}

	long exefs_expected_size = sizeof(exefs_header) + exefs_fsizes;

	if (siz != exefs_expected_size)
	{
		this->~ExeFS();
		throw InvalidFileSizeException(filename, siz, exefs_expected_size);
	}
}

bool ExeFS::ExeFS::verify()
{
	u8 hashcount = 9;
	u8 hash[0x20];

	for (u32 i = 0; i < this->file_entries.size(); i++)
	{
		hash_file_part(this->exefs_f, hash, sizeof(exefs_header) + this->file_entries[i]->file_offset, this->file_entries[i]->file_size);

		if (memcmp(this->header->file_hashes[hashcount], hash, 0x20) != 0)
			return false;

		hashcount--;
	}

	return true;
}

void ExeFS::export_entry(const char *filename, const char *entry_name)
{
	u32 len = strlen(entry_name);

	if (len > 8)
		throw InvalidEntryNameException(entry_name);

	for (u32 i = 0; i < this->file_entries.size(); i++)
	{
		if (strncmp(entry_name, this->file_entries[i]->file_name, len) == 0)
		{
			copy_file_part_buffered(this->exefs_f, filename, sizeof(exefs_header) + this->file_entries[i]->file_offset, this->file_entries[i]->file_size);
			return;
		}
	}

	throw EntryNotFoundException(entry_name);
}

ExeFS::~ExeFS()
{
	if (this->exefs_f != nullptr)
		fclose(this->exefs_f);

	if (this->header != nullptr)
		delete this->header;

	for (u32 i = 0; i < this->file_entries.size(); i++)
		delete this->file_entries[i];
}