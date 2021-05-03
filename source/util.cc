#include <util.hh>

std::string ctrpp::util::buffer_to_hex(u8 *data, u32 length, bool uppercase)
{
	const char *format = uppercase ? UPPER_HEX : LOWER_HEX;
	char tmp[3];
	std::string out;

	for (u32 i = 0; i < length; i++)
	{
		snprintf(tmp, 3, format, data[i]);

		out += std::string(tmp, 2);
	}

	return out;
}

u32 ctrpp::util::round_align_32(u32 in, u32 align)
{
	return (u32)std::ceil((double)in / align) * align;
}

u32 ctrpp::util::round_align_64(u64 in, u32 align)
{
	return (u32)std::ceil((double)in / align) * align;
}

void ctrpp::util::print_unicode(char16 *data, u32 length)
{
	std::u16string unc = std::u16string(data, length);
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;

	std::cout << conv.to_bytes(unc) << "\n";
}

long ctrpp::util::check_file(const char *filename)
{
	struct stat *st = new struct stat;
	long ret = -1;

	if (stat(filename, st) == 0)
	{
		if (!S_ISDIR(st->st_mode))
		{
			ret = st->st_size;
		}
	}

	delete st;
	return ret;
}

int get_remaining(int remaining)
{
	return ((remaining - BUF_SIZE) < 0) ? remaining : BUF_SIZE;
}

bool ctrpp::util::hash::sha256::hash_file_part(FILE *f, u8 *out_hash, u32 offset, u32 size)
{
	u8 *buf = new u8[BUF_SIZE];

	SHA256_CTX ctx = SHA256_CTX();

	int remaining = size;
	int read = 0;
	int to_read = get_remaining(remaining);

	if (!SHA256_Init(&ctx))
		goto failed;

	if (fseek(f, offset, 0) == -1)
		goto failed;

	while ((read = fread(buf, 1, to_read, f)) > 0)
	{
		if (read != to_read)
			goto failed;

		if (!SHA256_Update(&ctx, buf, to_read))
			goto failed;

		remaining -= read;
		to_read = get_remaining(remaining);
	}

	if (!SHA256_Final(out_hash, &ctx))
		goto failed;

	delete[] buf;
	return true;

failed:
	delete[] buf;
	return false;
}

int ctrpp::util::io::copy_file_part_buffered(FILE *in, FILE *out, u32 in_offset, u32 size, u32 out_offset, bool close_in, bool close_out)
{
	u8 *buf = new u8[BUF_SIZE];

	int remaining = size;
	int read = 0;
	int to_read = get_remaining(remaining);

	if (fseek(in, in_offset, 0) == -1)
		goto failed;

	if (fseek(out, out_offset, 0) == -1)
		goto failed;

	while ((read = fread(buf, 1, to_read, in)) > 0)
	{
		if (read != to_read)
			goto failed;

		if (!fwrite(buf, 1, read, out))
			goto failed;

		remaining -= read;
		to_read = get_remaining(remaining);
	}

	delete[] buf;
	return true;

failed:
	delete[] buf;
	return false;
}

int ctrpp::util::io::copy_file_part_buffered(FILE *in, const char *out, u32 offset, u32 size, bool close_in, bool close_out)
{
	FILE *out_f = fopen(out, "w");

	if (out_f == nullptr)
		return -1;

	u8 *buf = new u8[BUF_SIZE];

	int remaining = size;
	int read = 0;
	int to_read = get_remaining(remaining);

	if (fseek(in, offset, 0) == -1)
		goto failed;

	while ((read = fread(buf, 1, to_read, in)) > 0)
	{
		if (read != to_read)
			goto failed;

		if (!fwrite(buf, 1, read, out_f))
			goto failed;

		remaining -= read;
		to_read = get_remaining(remaining);
	}

	if (close_in)
		fclose(in);

	if (close_out)
		fclose(out_f);

	delete[] buf;
	return true;

failed:
	if (close_in)
		fclose(in);

	if (close_out)
		fclose(out_f);

	delete[] buf;
	return false;
}