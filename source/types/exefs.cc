#include <types/exefs.hh>

ctrpp::types::ExeFS::ExeFS::ExeFS()
{
}

ctrpp::types::ExeFS::ExeFS::ExeFS(const char *filename)
{
	FILE *exefs = nullptr;
	long siz = 0;
	int exefs_fsizes = 0;

	if ((siz = util::check_file(filename)) == -1)
		goto failed;

	exefs = fopen(filename, "r");

	if (exefs == nullptr)
		goto failed;

	this->exefs_f = exefs;
	this->header = new exefs_header;

	if (!fread(this->header, 1, sizeof(exefs_header), this->exefs_f))
		goto failed;

	this->file_entries = std::vector<exefs_file_entry *>();

	for (u32 i = 0; i < 10; i++)
	{
		if (memcmp((&(this->header->file_entries[i])), empty_exefs_entry, 0x10) != 0)
		{
			exefs_fsizes += ctrpp::util::round_align_32(this->header->file_entries[i].file_size, 0x200);

			exefs_file_entry *et = new exefs_file_entry;
			memcpy(et, (&(this->header->file_entries[i])), sizeof(exefs_file_entry));

			this->file_entries.push_back(et);
		}
	}

	if (siz != (long)((int)(sizeof(exefs_header)) + exefs_fsizes))
		goto failed;

	this->success_parsed = true;
	return;

failed:
	this->success_parsed = false;

	if (this->exefs_f != nullptr)
		fclose(this->exefs_f);
}

bool ctrpp::types::ExeFS::ExeFS::verify()
{
	u8 hashcount = 9;
	u8 *hash = new u8[0x20];

	for (u32 i = 0; i < this->file_entries.size(); i++)
	{
		if (!ctrpp::util::hash::sha256::hash_file_part(this->exefs_f, hash, sizeof(exefs_header) + this->file_entries[i]->file_offset, this->file_entries[i]->file_size))
			goto failed;

		if (memcmp(this->header->file_hashes[hashcount], hash, 0x20) != 0)
			goto failed;

		hashcount--;
	}

	delete[] hash;
	return true;

failed:
	delete[] hash;
	return false;
}

bool ctrpp::types::ExeFS::ExeFS::export_entry(const char *filename, const char *entry_name)
{
	u32 len = strlen(entry_name);

	if (len > 8)
		return false;

	bool found = false;
	exefs_file_entry *entry;

	for (u32 i = 0; i < this->file_entries.size(); i++)
	{
		if (strncmp(entry_name, this->file_entries[i]->file_name, len) == 0)
		{
			entry = this->file_entries[i];
			found = true;
			break;
		}
	}

	if (!found)
		return found;

	return util::io::copy_file_part_buffered(this->exefs_f, filename, sizeof(exefs_header) + entry->file_offset, entry->file_size, false, true);
}

ctrpp::types::ExeFS::ExeFS::~ExeFS()
{
	if (this->exefs_f != nullptr)
		fclose(this->exefs_f);

	if (this->header != nullptr)
		delete this->header;

	for (u32 i = 0; i < this->file_entries.size(); i++)
		delete this->file_entries[i];
}