#include <ctrpp/util.hh>

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

long ctrpp::util::io::check_file(const char *filename)
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

long get_hash_remaining(long remaining, u32 buf_size)
{
	return ((remaining - buf_size) < 0) ? remaining : buf_size;
}

int ctrpp::util::crypto::hash_buffer(u8 *buf, u64 size, u8 *out_hash)
{
	SHA256_CTX ctx = SHA256_CTX();

	if (!SHA256_Init(&ctx))
		return -1;

	if (!SHA256_Update(&ctx, buf, size))
		return -1;

	if (!SHA256_Final(out_hash, &ctx))
		return -1;

	return 0;
}

int ctrpp::util::crypto::hash_file(FILE *f, u64 size, u8 *out_hash)
{
	u8 *buf = new u8[REGULAR_HASH_BUFSIZE];

	SHA256_CTX ctx = SHA256_CTX();

	long remaining = size;
	long read = 0;
	long to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);

	if (!SHA256_Init(&ctx))
		goto failed;

	if (fseek(f, 0, 0) == -1)
		goto failed;

	printf("remaining: %lu | read: %lu | to_read: %lu\n", remaining, read, to_read);

	while ((read = fread(buf, 1, to_read, f)) > 0)
	{
		if (read != to_read)
			goto failed;

		if (!SHA256_Update(&ctx, buf, to_read))
			goto failed;

		remaining -= read;
		to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);

		printf("remaining: %lu | read: %lu | to_read: %lu\n", remaining, read, to_read);
	}

	if (!SHA256_Final(out_hash, &ctx))
		goto failed;

	delete[] buf;
	return 0;

failed:
	delete[] buf;
	return -1;
}

int ctrpp::util::crypto::hash_file_part(FILE *f, u8 *out_hash, u64 offset, u64 size)
{
	u8 *buf = new u8[REGULAR_HASH_BUFSIZE];

	SHA256_CTX ctx = SHA256_CTX();

	long remaining = size;
	long read = 0;
	long to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);

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
		to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);
	}

	if (!SHA256_Final(out_hash, &ctx))
		goto failed;

	delete[] buf;
	return 0;

failed:
	delete[] buf;
	return -1;
}

int ctrpp::util::crypto::hash_file_part_read(FILE *f, u8 *out_hash, u8 *out, u64 offset, u64 size)
{
	u8 *buf = new u8[REGULAR_HASH_BUFSIZE];

	SHA256_CTX ctx = SHA256_CTX();

	long remaining = size;
	long read = 0;
	long written = 0;
	long to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);

	if (!SHA256_Init(&ctx))
	{
		std::cout << "failed to initialize hashing context!\n";
		goto failed;
	}

	if (fseek(f, offset, 0) == -1)
	{
		std::cout << "failed to seek to offset 0!\n";
		goto failed;
	}

	while ((read = fread(buf, 1, to_read, f)) > 0)
	{
		memcpy(out + written, buf, read);

		if (read != to_read)
		{
			std::cout << "amount of read bytes is not equal to amount of bytes expected to read!\n";
			goto failed;
		}

		if (!SHA256_Update(&ctx, buf, to_read))
		{
			std::cout << "could not write data to hashing context!\n";
			goto failed;
		}

		remaining -= read;
		written += read;
		to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);
	}

	if (!SHA256_Final(out_hash, &ctx))
	{
		std::cout << "could not finalize hash! (could not write hash to hash buffer!)\n";
		goto failed;
	}	

	delete[] buf;
	return 0;

failed:
	delete[] buf;
	return -1;
}

int ctrpp::util::io::copy_file_part_buffered(FILE *in, FILE *out, u64 in_offset, u64 size, u64 out_offset, bool close_in, bool close_out)
{
	u8 *buf = new u8[REGULAR_HASH_BUFSIZE];

	long remaining = size;
	long read = 0;
	long to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);

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
		to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);
	}

	delete[] buf;
	return 0;

failed:
	delete[] buf;
	return -1;
}

int ctrpp::util::io::copy_file_part_buffered(FILE *in, const char *out, u64 offset, u64 size, bool close_in, bool close_out)
{
	FILE *out_f = fopen64(out, "w");

	if (out_f == nullptr)
		return -1;

	u8 *buf = new u8[REGULAR_HASH_BUFSIZE];

	long remaining = size;
	long read = 0;
	long to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);

	if (fseek(in, offset, 0) == -1)
		goto failed;

	while ((read = fread(buf, 1, to_read, in)) > 0)
	{
		if (read != to_read)
			goto failed;

		if (!fwrite(buf, 1, read, out_f))
			goto failed;

		remaining -= read;
		to_read = get_hash_remaining(remaining, REGULAR_HASH_BUFSIZE);
	}

	if (close_in)
		fclose(in);

	if (close_out)
		fclose(out_f);

	delete[] buf;
	return 0;

failed:
	if (close_in)
		fclose(in);

	if (close_out)
		fclose(out_f);

	delete[] buf;
	return -1;
}

