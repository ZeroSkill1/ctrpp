#include <types/ticket.hh>

ctrpp::types::Ticket::Ticket::Ticket()
{
}

ctrpp::types::Ticket::Ticket::Ticket(const char *filename)
{
	u32 sig_type;
	long streampos;
	u32 cindex_size;

	FILE *tik = fopen(filename, "r");

	if (tik == nullptr)
		goto failed;

	if (!fread(&sig_type, 1, 4, tik))
		goto failed;

	sig_type = _CTRPP_BE_U32(((u8 *)(&sig_type)));

	this->signature = new Signature(sig_type);

	if (!(this->signature->full_size > 0))
		goto failed;


	if (!fread(this->signature->sig, 1, this->signature->sig_size, tik))
		goto failed;
	if (!fread(this->signature->sig_padding, 1, this->signature->sig_padding_size, tik))
		goto failed;

	this->raw_ticket_data = new ticket_data;

	if (!fread(this->raw_ticket_data, 1, sizeof(ticket_data), tik))
		goto failed;

	if (!(streampos = ftell(tik)))
		goto failed;

	if (fseek(tik, streampos + 4, SEEK_SET) == -1) // seek to cindex size pos
		goto failed;

	if (!fread(&cindex_size, 1, 4, tik))
		goto failed;

	cindex_size = _CTRPP_BE_U32(((u8 *)(&cindex_size)));

	this->content_index = new TicketContentIndex(cindex_size);

	if (fseek(tik, streampos, 0) == -1)
		goto failed;

	if (!fread(this->content_index->content_index_data, 1, this->content_index->size, tik))
		goto failed;

	fclose(tik);

	this->success_parsed = true;

	return;

	failed:

	if (tik != nullptr)
		fclose(tik);

	this->success_parsed = false;

	return;
}

u64 ctrpp::types::Ticket::Ticket::ticket_id()
{
	return _CTRPP_BE_U64(((u8 *)(&(this->raw_ticket_data->ticket_id))));
}

u32 ctrpp::types::Ticket::Ticket::console_id()
{
	return _CTRPP_BE_U32(((u8 *)(&(this->raw_ticket_data->console_id))));
}

u64 ctrpp::types::Ticket::Ticket::title_id()
{
	return _CTRPP_BE_U64(((u8 *)(&(this->raw_ticket_data->title_id))));
}

u16 ctrpp::types::Ticket::Ticket::ticket_title_version()
{
	return _CTRPP_BE_U16(((u8 *)(&(this->raw_ticket_data->ticket_title_version))));
}

u32 ctrpp::types::Ticket::Ticket::eshop_account_id()
{
	return _CTRPP_BE_U32(((u8 *)(&(this->raw_ticket_data->eshop_account_id))));
}

ctrpp::types::Ticket::Ticket::~Ticket()
{
	if (!(this->signature == nullptr)) delete this->signature;
	if (!(this->raw_ticket_data == nullptr)) delete this->raw_ticket_data;
	if (!(this->content_index == nullptr)) delete this->content_index;
}


ctrpp::types::Ticket::TicketContentIndex::TicketContentIndex()
{

}

ctrpp::types::Ticket::TicketContentIndex::TicketContentIndex(u32 size)
{
	this->size = size;
	this->content_index_data = new u8[size];
}

ctrpp::types::Ticket::TicketContentIndex::~TicketContentIndex()
{
	if (!(this->content_index_data == nullptr)) delete [] this->content_index_data;
}