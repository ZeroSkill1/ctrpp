#include <types/smdh.hh>

ctrpp::types::SMDH::SMDH::SMDH()
{
}

ctrpp::types::SMDH::SMDH::SMDH(const char *filename)
{
	struct stat *st = nullptr;
	FILE *smdh = nullptr;

	if (!ctrpp::util::check_file(filename, st))
		goto failed;

	smdh = fopen(filename, "r");

	if (smdh == nullptr)
		goto failed;

	this->raw_smdh_data = new smdh_data;

	if(!fread(this->raw_smdh_data, 1, sizeof(smdh_data), smdh))
		goto failed;

	delete st;
	fclose(smdh);

	this->success_parsed = true;

	return;

failed:

	delete st;

	if (smdh != nullptr)
		fclose(smdh);

	this->success_parsed = false;

	return;
}

ctrpp::types::SMDH::SMDH::~SMDH()
{
	if (this->raw_smdh_data != nullptr)
		delete this->raw_smdh_data;
}

u32 ctrpp::types::SMDH::SMDH::region_lockout()
{
	return _CTRPP_LE_U32(BYTES(this->raw_smdh_data->region_lockout));
}

u32 ctrpp::types::SMDH::SMDH::match_maker_id()
{
	return _CTRPP_LE_U32(BYTES(this->raw_smdh_data->match_maker_id));
}

u64 ctrpp::types::SMDH::SMDH::match_maker_bit_id()
{
	return _CTRPP_LE_U64(BYTES(this->raw_smdh_data->match_maker_bit_id));
}

u16 ctrpp::types::SMDH::SMDH::eula_version()
{
	return _CTRPP_LE_U16(BYTES(this->raw_smdh_data->eula_version));
}

float ctrpp::types::SMDH::SMDH::default_banner_animation_frame()
{
	return ints::le_float(BYTES(this->raw_smdh_data->default_banner_animation_frame));
}

u32 ctrpp::types::SMDH::SMDH::cec_id()
{
	return _CTRPP_LE_U32(BYTES(this->raw_smdh_data->cec_id));
}