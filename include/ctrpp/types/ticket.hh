#ifndef __ticket_hh
#define __ticket_hh

#include <string.h>
#include <stdio.h>

#include <ctrpp/exceptions.hh>
#include <ctrpp/ints.hh>
#include <ctrpp/sig.hh>
#include <ctrpp/util/io.hh>

namespace ctrpp
{
	namespace types
	{
		namespace Ticket
		{
			typedef struct __attribute__((packed)) ticket_data
			{
				char issuer[0x40];
				u8 ecc_pubkey[0x3C];
				u8 version;
				u8 ca_crl_version;
				u8 signer_crl_version;
				u8 titlekey[0x10];
				u8 reserved;
				u64 ticket_id;
				u32 console_id;
				u64 title_id;
				u16 reserved_1;
				u16 ticket_title_version;
				u64 reserved_2;
				u8 license_type;
				u8 common_key_index;
				u8 reserved_3[0x2A];
				u32 eshop_account_id;
				u8 reserved_4;
				u8 audit;
				u8 reserved_5[0x42];
				u8 limits[0x40];

			} ticket_data;

			class TicketContentIndex
			{
			public:
				u32 size;
				u8 *content_index_data = nullptr;

				TicketContentIndex();
				TicketContentIndex(u32 size);
				~TicketContentIndex();
			};

			class Ticket
			{
			public:
				Signature *signature = nullptr;
				ticket_data *raw_ticket_data = nullptr;
				TicketContentIndex *content_index = nullptr;

				Ticket();
				Ticket(const char *filename);

				u64 ticket_id();
				u32 console_id();
				u64 title_id();
				u16 ticket_title_version();
				u32 eshop_account_id();

				~Ticket();
			};
		}
	}
}

#endif