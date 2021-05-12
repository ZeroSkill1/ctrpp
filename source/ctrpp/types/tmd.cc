#include <ctrpp/types/tmd.hh>

using namespace ctrpp::exceptions::sig;
using namespace ctrpp::exceptions::fs;
using namespace ctrpp::types::TMD;
using namespace ctrpp::util::io;

//tmd

TMD::TMD()
{
}

TMD::TMD(const char *filename)
{
	u8 buf[CHUNK_RECORD_SIZE]; //using chunk record size, because this buffer will be used to copy both chunk AND info records.
	u32 sig_type;

	FILE *f = nullptr;

	f = open_file_nsz(filename, "r");

	if (fread(&sig_type, 1, 4, f) != 4)
	{
		fclose(f);

		throw FileReadException(filename);
	}

	sig_type = _CTRPP_BE_U32(BYTES(sig_type));

	this->signature = new ctrpp::Signature(sig_type);

	if (this->signature->full_size == -1) //return bad if sig type is not supported
	{
		delete this->signature;

		fclose(f);

		throw InvalidSignatureTypeException(sig_type);
	}

	this->raw_tmd_header_data = new tmd_header;

	if (
		fread(this->signature->sig, 1, this->signature->sig_size, f) != this->signature->sig_size ||
		fread(this->signature->sig_padding, 1, this->signature->sig_padding_size, f) != this->signature->sig_padding_size ||
		fread(this->raw_tmd_header_data, 1, sizeof(tmd_header), f) != sizeof(tmd_header))
	{
		delete this->signature;
		delete this->raw_tmd_header_data;

		fclose(f);

		throw FileReadException(filename);
	}

	//read content info records

	this->ContentInfoRecords = std::vector<ContentInfoRecord *>();

	for (u32 i = 0; i < INFO_RECORD_AMOUNT; i++)
	{
		if (fread(buf, 1, INFO_RECORD_SIZE, f) != INFO_RECORD_SIZE)
		{
			delete this->signature;
			delete this->raw_tmd_header_data;

			delete_info_records();

			fclose(f);

			throw FileReadException(filename);
		}

		if (!(memcmp(buf, EMPTY_INFO_RECORD, INFO_RECORD_SIZE) == 0))
		{
			tmd_content_info_record *record = new tmd_content_info_record;

			memcpy(record, buf, INFO_RECORD_SIZE);

			this->ContentInfoRecords.push_back(new ContentInfoRecord(record));
		}
	}

	//read content chunk records

	this->ContentChunkRecords = std::vector<ContentChunkRecord *>();

	for (u16 i = 0; i < this->content_count(); i++)
	{
		if (fread(buf, 1, CHUNK_RECORD_SIZE, f) != CHUNK_RECORD_SIZE)
		{
			delete this->signature;
			delete this->raw_tmd_header_data;

			delete_info_records();
			delete_chunk_records();

			fclose(f);

			throw FileReadException(filename);
		}

		tmd_content_chunk_record *chunk = new tmd_content_chunk_record;

		memcpy(chunk, buf, CHUNK_RECORD_SIZE);

		ContentChunkRecord *record = new ContentChunkRecord(chunk);
		this->ContentChunkRecords.push_back(record);
	}

	fclose(f);
}

u64 TMD::title_id()
{
	return _CTRPP_BE_U64(BYTES(this->raw_tmd_header_data->title_id));
}

u32 TMD::title_type()
{
	return _CTRPP_BE_U32(BYTES(this->raw_tmd_header_data->title_type));
}

u16 TMD::group_id()
{
	return _CTRPP_BE_U16(BYTES(this->raw_tmd_header_data->group_id));
}

u32 TMD::save_size()
{
	return _CTRPP_LE_U32(BYTES(this->raw_tmd_header_data->save_size));
}

u32 TMD::srl_private_save_data_size()
{
	return _CTRPP_LE_U32(BYTES(this->raw_tmd_header_data->srl_private_save_data_size));
}

u16 TMD::title_version()
{
	return _CTRPP_BE_U16(BYTES(this->raw_tmd_header_data->title_version));
}

u16 TMD::content_count()
{
	return _CTRPP_BE_U16(BYTES(this->raw_tmd_header_data->content_count));
}

u16 TMD::boot_content()
{
	return _CTRPP_BE_U16(BYTES(this->raw_tmd_header_data->boot_content));
}

TMD::~TMD()
{
	if (this->raw_tmd_header_data != nullptr)
		delete this->raw_tmd_header_data;
	if (this->signature != nullptr)
		delete this->signature;

	this->delete_info_records();
	this->delete_chunk_records();
}

void TMD::delete_info_records()
{
	for (size_t i = 0; i < this->ContentInfoRecords.size(); i++)
		delete this->ContentInfoRecords[i];
}

void TMD::delete_chunk_records()
{
	for (size_t i = 0; i < this->ContentChunkRecords.size(); i++)
		delete this->ContentChunkRecords[i];
}

//content info record

ContentInfoRecord::ContentInfoRecord()
{
}

ContentInfoRecord::ContentInfoRecord(tmd_content_info_record *info)
{
	this->raw_info_record_data = info;
}

u16 ContentInfoRecord::content_index_offset()
{
	return _CTRPP_BE_U16(BYTES(this->raw_info_record_data->content_index_offset));
}

u16 ContentInfoRecord::content_command_count()
{
	return _CTRPP_BE_U16(BYTES(this->raw_info_record_data->content_command_count));
}

ContentInfoRecord::~ContentInfoRecord()
{
	if (this->raw_info_record_data != nullptr)
		delete this->raw_info_record_data;
}

//content chunk record

ContentChunkRecord::ContentChunkRecord()
{
}

ContentChunkRecord::ContentChunkRecord(tmd_content_chunk_record *chunk)
{
	this->raw_chunk_record_data = chunk;
}

u32 ContentChunkRecord::content_id()
{
	return _CTRPP_BE_U32(BYTES(this->raw_chunk_record_data->content_id));
}

u16 ContentChunkRecord::content_index()
{
	return _CTRPP_BE_U16(BYTES(this->raw_chunk_record_data->content_index));
}

u16 ContentChunkRecord::content_type()
{
	return _CTRPP_BE_U16(BYTES(this->raw_chunk_record_data->content_type));
}

u64 ContentChunkRecord::content_size()
{
	return _CTRPP_BE_U64(BYTES(this->raw_chunk_record_data->content_size));
}

ContentChunkRecord::~ContentChunkRecord()
{
	if (this->raw_chunk_record_data != nullptr)
		delete this->raw_chunk_record_data;
}