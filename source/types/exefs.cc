#include <types/exefs.hh>

ctrpp::types::ExeFS::ExeFS::ExeFS()
{

}

ctrpp::types::ExeFS::ExeFS::ExeFS(const char *filename)
{
	struct stat *st = nullptr;
	FILE *exefs = nullptr;
	int exefs_fsizes = 0;

	if (!ctrpp::util::check_file(filename, st))
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

if (st->st_size != (long)((int)(sizeof(exefs_header)) + exefs_fsizes))
	goto failed;

	this->success_parsed = true;
	delete st;
	return;

	failed:
	this->success_parsed = false;

	delete st;

	if (this->exefs_f != nullptr)
		fclose(this->exefs_f);
}

ctrpp::types::ExeFS::ExeFS::~ExeFS()
{
	if (this->exefs_f != nullptr)
		fclose(this->exefs_f);

	if (this->header != nullptr)
		delete this->header;

	for (u32 i = 0; i < this->file_entries.size(); i++)
	{
		delete this->file_entries[i];
	}
}