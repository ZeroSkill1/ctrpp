#include <ctrpp/util/io.hh>

using namespace ctrpp::exceptions::crypto;
using namespace ctrpp::exceptions::fs;


FILE *ctrpp::util::io::open_file_nsz(const char *filename, const char *mode)
{
	struct stat st;
		
	if (stat(filename, &st) != 0)
		throw FileNotFoundException(filename);

	if (S_ISDIR(st.st_mode))
		throw PathIsDirectoryException(filename);

	return fopen(filename, mode);
}

FILE *ctrpp::util::io::open_file(const char *filename, const char *mode, long *out_size)
{
	struct stat st;
		
	if (stat(filename, &st) != 0)
		throw FileNotFoundException(filename);

	if (S_ISDIR(st.st_mode))
		throw PathIsDirectoryException(filename);

	*out_size = st.st_size;

	return fopen(filename, mode);
}

long get_remaining(long remaining)
{
	return ((remaining - BUFSIZE) < 0) ? remaining : BUFSIZE;
}

void ctrpp::util::crypto::hash_buffer(u8 *buf, u64 size, u8 *out_hash)
{
	SHA256_CTX ctx;
	int ctx_err_code = 0;

	if (!(ctx_err_code = SHA256_Init(&ctx)))
		throw HashInitException(ctx_err_code);

	if (!(ctx_err_code = SHA256_Update(&ctx, buf, size)))
		throw HashUpdateException(ctx_err_code);

	if (!(ctx_err_code = SHA256_Final(out_hash, &ctx)))
		throw HashFinalizeException(ctx_err_code);
}

void ctrpp::util::crypto::hash_file(FILE *f, size_t size, u8 *out_hash)
{
	u8 buf[BUFSIZE];

	SHA256_CTX ctx;
	size_t ctx_res = 0;

	off_t remaining = size;
	off_t read = 0;
	off_t to_read = get_remaining(remaining);

	if (!(ctx_res = SHA256_Init(&ctx)))
		throw HashInitException(ctx_res);

	if (fseek(f, 0, 0) == -1)
		throw FileSeekException(0);

	while ((read = fread(buf, 1, to_read, f)) > 0)
	{
		if (read != to_read)
			throw FileReadException();

		if (!(ctx_res = SHA256_Update(&ctx, buf, to_read)))
			throw HashUpdateException(ctx_res);

		remaining -= read;
		to_read = get_remaining(remaining);
	}

	if (!(ctx_res = SHA256_Final(out_hash, &ctx)))
		throw HashFinalizeException(ctx_res);
}

void ctrpp::util::crypto::hash_file_part(FILE *f, u8 *out_hash, off_t offset, size_t size)
{
	u8 buf[BUFSIZE];

	SHA256_CTX ctx;
	size_t ctx_res = 0;

	off_t remaining = size;
	off_t read = 0;
	off_t to_read = get_remaining(remaining);

	if (!(ctx_res = SHA256_Init(&ctx)))
		throw HashInitException(ctx_res);

	if (fseek(f, 0, 0) == -1)
		throw FileSeekException(0);

	while ((read = fread(buf, 1, to_read, f)) > 0)
	{
		if (read != to_read)
			throw FileReadException();

		if (!(ctx_res = SHA256_Update(&ctx, buf, to_read)))
			throw HashUpdateException(ctx_res);

		remaining -= read;
		to_read = get_remaining(remaining);
	}

	if (!SHA256_Final(out_hash, &ctx))
		throw HashFinalizeException(ctx_res);
}

void ctrpp::util::crypto::hash_file_part_read(FILE *f, u8 *out_hash, u8 *out, off_t offset, size_t size)
{
	u8 buf[BUFSIZE];

	SHA256_CTX ctx;
	size_t ctx_res = 0;

	long remaining = size;
	long read = 0;
	long written = 0;
	long to_read = get_remaining(remaining);

	if (!(ctx_res = SHA256_Init(&ctx)))
		throw HashInitException(ctx_res);

	if (fseek(f, offset, 0) == -1)
		throw FileSeekException(offset);

	while ((read = fread(buf, 1, to_read, f)) > 0)
	{
		memcpy(out + written, buf, read);

		if (read != to_read)
			throw FileReadException();

		if (!(ctx_res = SHA256_Update(&ctx, buf, to_read)))
			throw HashUpdateException(ctx_res);

		remaining -= read;
		written += read;
		to_read = get_remaining(remaining);
	}

	if (!(ctx_res = SHA256_Final(out_hash, &ctx)))
		throw HashFinalizeException(ctx_res);
}

void ctrpp::util::io::copy_file_part_buffered(FILE *in, FILE *out, off_t in_offset, size_t size, off_t out_offset, bool close_in, bool close_out)
{
	u8 buf[BUFSIZE];

	long remaining = size;
	long read = 0;
	long to_read = get_remaining(remaining);

	if (fseek(in, in_offset, 0) == -1)
		throw FileException(std::string("Failed to seek infile to offset (" + std::to_string(in_offset) + ")"));

	if (fseek(out, out_offset, 0) == -1)
		throw FileException(std::string("Failed to seek outfile to offset (" + std::to_string(out_offset) + ")"));

	while ((read = fread(buf, 1, to_read, in)) > 0)
	{
		if (read != to_read)
			throw FileException("Failed to read from infile");

		if (!fwrite(buf, 1, read, out))
			throw FileException("Failed to write to outfile");

		remaining -= read;
		to_read = get_remaining(remaining);
	}

	if (close_in)
		fclose(in);

	if (close_out)
		fclose(out);
}

void ctrpp::util::io::copy_file_part_buffered(FILE *in, const char *out, off_t offset, size_t size, bool close_in, bool close_out)
{
	FILE *out_f = fopen64(out, "w");

	if (out_f == nullptr)
		throw FileException("Could not create outfile at \"" + std::string(out) + "\"");

	u8 buf[BUFSIZE];

	long remaining = size;
	long read = 0;
	long to_read = get_remaining(remaining);

	if (fseek(in, offset, 0) == -1)
		throw FileSeekException(0);

	while ((read = fread(buf, 1, to_read, in)) > 0)
	{
		if (read != to_read)
			throw FileException("Failed to read from infile");

		if (!fwrite(buf, 1, read, out_f))
			throw FileException("Failed to write to outfile");

		remaining -= read;
		to_read = get_remaining(remaining);
	}

	if (close_in)
		fclose(in);

	if (close_out)
		fclose(out_f);
}

