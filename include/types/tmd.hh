#ifndef __tmd_hh
#define __tmd_hh

#include <string.h>
#include <stdio.h>
#include <vector>

#include <ints.hh>
#include <sig.hh>

namespace ctrpp
{
	namespace types
	{
		namespace TMD
		{
			static const u8 empty_inforecord[0x24] = 
			{
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			};

			typedef const enum class ContentTypeFlags
			{
				Encrypted = 0x1,
				Disc      = 0x2,
				CFM       = 0x4,
				Optional  = 0x4000,
				Shared    = 0x8000
			};

			typedef struct __attribute__((packed)) tmd_header
			{
				char issuer[0x40];
				u8 version;
				u8 ca_crl_version;
				u8 signer_crl_version;
				u8 reserved_0;
				u64 system_version;				// BE
				u64 title_id;					// BE
				u32 title_type;					// BE
				u16 group_id;					// BE
				u32 save_size;					// LE, also SRL Public Save Data Size
				u32 srl_private_save_data_size; // LE
				u32 reserved_1;
				u8 srl_flag;
				u8 reserved_2[0x31];
				u8 access_rights[4]; // BE
				u16 title_version;	 // BE
				u16 content_count;	 // BE
				u16 boot_content;	 // BE
				u16 padding;
				u8 content_info_records_hash[0x20]; //sha256
			} tmd_header;

			typedef struct __attribute__((packed)) tmd_content_info_record
			{
				u16 content_index_offset;  // BE
				u16 content_command_count; // BE
				u8 hash[0x20];
			} tmd_content_info_record;

			typedef struct __attribute__((packed)) tmd_content_chunk_record
			{
				u32 content_id;	   // BE
				u16 content_index; // BE
				u16 content_type;  // BE, flags
				u64 content_size;  // BE
				u8 hash[0x20];
			} tmd_content_chunk_record;

			class ContentInfoRecord
			{
			public:
				tmd_content_info_record *raw_info_record_data = nullptr;

				ContentInfoRecord();
				ContentInfoRecord(tmd_content_info_record *info);

				u16 contnet_index_offset();
				u16 content_command_count();

				~ContentInfoRecord();
			};

			class ContentChunkRecord
			{
			public:
				tmd_content_chunk_record *raw_chunk_record_data = nullptr;

				ContentChunkRecord();
				ContentChunkRecord(tmd_content_chunk_record *chunk);

				u32 content_id();
				u16 content_index();
				u16 content_type();
				u64 content_size();

				~ContentChunkRecord();
			};

			class TMD
			{
			public:
				Signature *signature = nullptr;
				tmd_header *raw_tmd_header_data = nullptr;

				std::vector<ContentInfoRecord *> ContentInfoRecords;
				std::vector<ContentChunkRecord *> ContentChunkRecords;

				bool success_parsed;

				TMD(const char *filename);
				TMD();

				u64 system_version();
				u64 title_id();
				u32 title_type();
				u16 group_id();
				u32 save_size();
				u32 srl_private_save_data_size();
				u16 title_version();
				u16 content_count();
				u16 boot_content();

				~TMD();
			};
		}
	}
}

#endif