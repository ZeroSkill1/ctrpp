#include <ctrpp/types/smdh.hh>

using namespace ctrpp::exceptions::fs;
using namespace ctrpp::types::SMDH;
using namespace ctrpp::util::io;

SMDH::SMDH()
{
}

SMDH::SMDH(const char *filename)
{
	FILE *smdh = nullptr;

	off_t siz = 0;

	smdh = open_file(filename, "r", &siz);

	if (siz != sizeof(smdh_data))
	{
		fclose(smdh);
		throw InvalidFileSizeException(filename, siz, sizeof(smdh_data));
	}

	this->raw_smdh_data = new smdh_data;

	if (!fread(this->raw_smdh_data, 1, sizeof(smdh_data), smdh))
	{
		this->~SMDH();

		fclose(smdh);
		
		throw FileReadException();
	}

	fclose(smdh);
}

SMDH::~SMDH()
{
	if (this->raw_smdh_data != nullptr)
		delete this->raw_smdh_data;
}

u32 SMDH::SMDH::region_lockout()
{
	return _CTRPP_LE_U32(BYTES(this->raw_smdh_data->region_lockout));
}

u32 SMDH::SMDH::match_maker_id()
{
	return _CTRPP_LE_U32(BYTES(this->raw_smdh_data->match_maker_id));
}

u64 SMDH::SMDH::match_maker_bit_id()
{
	return _CTRPP_LE_U64(BYTES(this->raw_smdh_data->match_maker_bit_id));
}

u16 SMDH::SMDH::eula_version()
{
	return _CTRPP_LE_U16(BYTES(this->raw_smdh_data->eula_version));
}

float SMDH::SMDH::default_banner_animation_frame()
{
	return ints::le_float(BYTES(this->raw_smdh_data->default_banner_animation_frame));
}

u32 SMDH::SMDH::cec_id()
{
	return _CTRPP_LE_U32(BYTES(this->raw_smdh_data->cec_id));
}