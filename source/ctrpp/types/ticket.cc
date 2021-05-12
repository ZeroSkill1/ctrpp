#include <ctrpp/types/ticket.hh>

using namespace ctrpp::exceptions::fs;
using namespace ctrpp::types::Ticket;
using namespace ctrpp::util::io;

Ticket::Ticket()
{
}

Ticket::Ticket(const char *filename)
{
	FILE *tik = nullptr;
	u32 tmp;
	s32 tmp2;

	tik = open_file_nsz(filename, "r");

	if (fread(&tmp, 1, 4, tik) != 4)
	{
		fclose(tik);

		throw FileReadException(filename);
	}

	tmp = _CTRPP_BE_U32(BYTES(tmp));

	this->signature = new Signature(tmp);
	this->raw_ticket_data = new ticket_data;

	if (
		fread(this->signature->sig, 1, this->signature->sig_size, tik) != this->signature->sig_size ||
	    fread(this->signature->sig_padding, 1, this->signature->sig_padding_size, tik) != this->signature->sig_padding_size ||
		fread(this->raw_ticket_data, 1, sizeof(ticket_data), tik) != sizeof(ticket_data) ||
		fread(&tmp, 1, 4, tik) != 4 ||
		fread(&tmp, 1, 4, tik) != 4
	)
	{
		delete this->signature; 
		delete this->raw_ticket_data;

		fclose(tik);

		throw FileReadException(filename);
	}

	tmp = _CTRPP_BE_U32(BYTES(tmp));

	this->content_index = new TicketContentIndex(tmp);

	if ((tmp2 = ftell(tik)) == -1)
	{
		delete this->signature; 
		delete this->raw_ticket_data;
		delete this->content_index;

		fclose(tik);

		throw FileException("Could not get position of file stream");
	}

	if (fseek(tik, tmp2 - 1, 0) == -1)
	{
		delete this->signature; 
		delete this->raw_ticket_data;
		delete this->content_index;

		fclose(tik);

		throw FileSeekException(filename, tmp2 - 1);
	}

	if (fread(this->content_index->content_index_data, 1, this->content_index->size, tik) != this->content_index->size)
	{
		delete this->signature; 
		delete this->raw_ticket_data;
		delete this->content_index;

		fclose(tik);

		throw FileReadException(filename);
	}

	fclose(tik);
}

u64 Ticket::Ticket::ticket_id()
{
	return _CTRPP_BE_U64(BYTES(this->raw_ticket_data->ticket_id));
}

u32 Ticket::Ticket::console_id()
{
	return _CTRPP_BE_U32(BYTES(this->raw_ticket_data->console_id));
}

u64 Ticket::Ticket::title_id()
{
	return _CTRPP_BE_U64(BYTES(this->raw_ticket_data->title_id));
}

u16 Ticket::ticket_title_version()
{
	return _CTRPP_BE_U16(BYTES(this->raw_ticket_data->ticket_title_version));
}

u32 Ticket::eshop_account_id()
{
	return _CTRPP_BE_U32(BYTES(this->raw_ticket_data->eshop_account_id));
}

Ticket::~Ticket()
{
	if (!(this->signature == nullptr))
		delete this->signature;
	if (!(this->raw_ticket_data == nullptr))
		delete this->raw_ticket_data;
	if (!(this->content_index == nullptr))
		delete this->content_index;
}

TicketContentIndex::TicketContentIndex()
{
}

TicketContentIndex::TicketContentIndex(u32 size)
{
	this->size = size;
	this->content_index_data = new u8[size];
}

TicketContentIndex::~TicketContentIndex()
{
	if (!(this->content_index_data == nullptr))
		delete[] this->content_index_data;
}