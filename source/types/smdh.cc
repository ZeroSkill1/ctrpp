#include <types/smdh.hh>
#include <sys/stat.h>

ctrpp::types::SMDH::SMDH::SMDH()
{
}

ctrpp::types::SMDH::SMDH::SMDH(const char *filename)
{
	struct stat *st = new struct stat;
	FILE *smdh = nullptr;

	if (stat(filename, st) != 0)
		goto failed;
	
	printf("passed stat\n");

	if (st->st_size != sizeof(smdh_data))
		goto failed;

	printf("passed stat verify\n");

	smdh = fopen(filename, "r");

	if (smdh == nullptr)
		goto failed;

	printf("passed open file\n");

	this->raw_smdh_data = new smdh_data;

	printf("allocated data mem\n");

	if(!fread(this->raw_smdh_data, 1, sizeof(smdh_data), smdh))
		goto failed;

	printf("read file into data mem\n");

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