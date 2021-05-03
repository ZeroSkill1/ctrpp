#include <types/tmd.hh>

//tmd

ctrpp::types::TMD::TMD::TMD()
{
}

ctrpp::types::TMD::TMD::TMD(const char *filename)
{
	u32 sig_type;
	u8 *cir_buf = new u8[0x24];

	FILE *f = fopen(filename, "r");

	if (f == nullptr)
		goto failed;

	if (!fread(&sig_type, 1, 4, f))
		goto failed;

	sig_type = _CTRPP_BE_U32(((u8 *)(&sig_type)));

	this->signature = new ctrpp::Signature(sig_type);

	if (!(this->signature->full_size > 0)) //return bad if sig is not supported
		goto failed;
	if (!fread(this->signature->sig, 1, this->signature->sig_size, f))
		goto failed;
	if (!fread(this->signature->sig_padding, 1, this->signature->sig_padding_size, f))
		goto failed;	
	this->raw_tmd_header_data = new tmd_header;
	if (!fread(this->raw_tmd_header_data, 1, sizeof(tmd_header), f))
		goto failed;

	//read content info records

	this->ContentInfoRecords = std::vector<ContentInfoRecord *>();

	for (u32 i = 0; i < 64; i++)
	{
		if (!(fread(cir_buf, 1, 0x24, f) > 0))
		{
			goto failed;
		}

		if (!(memcmp(cir_buf, empty_inforecord, 0x24) == 0))
		{
			tmd_content_info_record *record = new tmd_content_info_record;

			memcpy(record, cir_buf, 0x24);

			this->ContentInfoRecords.push_back(new ContentInfoRecord(record));
		}
	}

	delete [] cir_buf;

	//read content chunk records

	this->ContentChunkRecords = std::vector<ContentChunkRecord *>();

	for (u16 i = 0; i < this->content_count(); i++)
	{
		tmd_content_chunk_record *chunk = new tmd_content_chunk_record;

		if ((fread(chunk, 1, 0x30, f) > 0))
		{
			this->ContentChunkRecords.push_back(new ContentChunkRecord(chunk));
		}
		else 
		{
			delete chunk;
			goto failed;
		}
	}

	fclose(f);

	this->success_parsed = true;
	return;

failed:
	if (f != nullptr)
		fclose(f);

	if (cir_buf != nullptr) 
		delete [] cir_buf;

	this->success_parsed = false;
	return;
}

u64 ctrpp::types::TMD::TMD::title_id()
{
	return _CTRPP_BE_U64(((u8 *)(&(this->raw_tmd_header_data->title_id))));
}

u32 ctrpp::types::TMD::TMD::title_type()
{
	return _CTRPP_BE_U32(((u8 *)(&(this->raw_tmd_header_data->title_type))));
}

u16 ctrpp::types::TMD::TMD::group_id()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_tmd_header_data->group_id))));
}

//le stuff

u32 ctrpp::types::TMD::TMD::save_size()
{
	return _CTRPP_LE_U32(((u8 *)(&(this->raw_tmd_header_data->save_size))));
}

u32 ctrpp::types::TMD::TMD::srl_private_save_data_size()
{
	return _CTRPP_LE_U32(((u8 *)(&(this->raw_tmd_header_data->srl_private_save_data_size))));
}

//end le stuff

u16 ctrpp::types::TMD::TMD::title_version()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_tmd_header_data->title_version))));
}

u16 ctrpp::types::TMD::TMD::content_count()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_tmd_header_data->content_count))));
}

u16 ctrpp::types::TMD::TMD::boot_content()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_tmd_header_data->boot_content))));
}

ctrpp::types::TMD::TMD::~TMD()
{
	if (this->raw_tmd_header_data != nullptr) delete this->raw_tmd_header_data;
	if (this->signature != nullptr) delete this->signature;

	for (u32 i = 0; i < this->ContentInfoRecords.size(); i++)
	{
		delete this->ContentInfoRecords[i];
	}

	for (u32 i = 0; i < this->ContentChunkRecords.size(); i++)
	{
		delete this->ContentChunkRecords[i];
	}
}

//end tmd

//start content info record

ctrpp::types::TMD::ContentInfoRecord::ContentInfoRecord()
{
}

ctrpp::types::TMD::ContentInfoRecord::ContentInfoRecord(tmd_content_info_record *info)
{
	this->raw_info_record_data = info;
}

u16 ctrpp::types::TMD::ContentInfoRecord::contnet_index_offset()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_info_record_data->content_index_offset))));
}

u16 ctrpp::types::TMD::ContentInfoRecord::content_command_count()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_info_record_data->content_command_count))));
}

ctrpp::types::TMD::ContentInfoRecord::~ContentInfoRecord()
{
	if (this->raw_info_record_data != nullptr) delete this->raw_info_record_data;
}

//end content info record

//start content chunk record

ctrpp::types::TMD::ContentChunkRecord::ContentChunkRecord()
{	
}

ctrpp::types::TMD::ContentChunkRecord::ContentChunkRecord(tmd_content_chunk_record *chunk)
{
	this->raw_chunk_record_data = chunk;
}

u32 ctrpp::types::TMD::ContentChunkRecord::content_id()
{
	return _CTRPP_BE_U32(((u8 *)(&(this->raw_chunk_record_data->content_id))));
}

u16 ctrpp::types::TMD::ContentChunkRecord::content_index()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_chunk_record_data->content_index))));
}

u16 ctrpp::types::TMD::ContentChunkRecord::content_type()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_chunk_record_data->content_type))));
}

u64 ctrpp::types::TMD::ContentChunkRecord::content_size()
{
	return _CTRPP_BE_U64(((u8 *)(&(this->raw_chunk_record_data->content_size))));
}

ctrpp::types::TMD::ContentChunkRecord::~ContentChunkRecord()
{
	if (this->raw_chunk_record_data != nullptr) delete this->raw_chunk_record_data;
}

//end content info record