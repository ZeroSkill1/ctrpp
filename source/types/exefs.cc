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
	u8 *buf = new u8[BUF_SIZE];
	u8 *hash = new u8[0x20];

	for (u32 i = 0; i < this->file_entries.size(); i++)
	{
		SHA256_CTX ctx = SHA256_CTX();
		if (!SHA256_Init(&ctx))
		{
			goto failed;
		}

		if (fseek(this->exefs_f, this->file_entries[i]->file_offset + sizeof(exefs_header), 0) == -1)
		{
			goto failed;
		}

		int remaining = this->file_entries[i]->file_size;
		int read = 0;

		int to_read = ((remaining - BUF_SIZE) < 0) ? remaining : BUF_SIZE;

		while ((read = fread(buf, 1, to_read, this->exefs_f)) > 0)
		{
			if (read != to_read)
			{
				goto failed;
			}

			if (!SHA256_Update(&ctx, buf, to_read))
			{
				goto failed;
			}

			remaining -= read;
			to_read = ((remaining - BUF_SIZE) < 0) ? remaining : BUF_SIZE;
		}

		if (!SHA256_Final(hash, &ctx))
		{
			goto failed;
		}

		if (memcmp(this->header->file_hashes[hashcount], hash, 0x20) != 0)
		{
			goto failed;
		}

		hashcount--;
	}

	delete[] buf;
	delete[] hash;
	return true;

failed:
	delete[] buf;
	delete[] hash;
	return false;
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