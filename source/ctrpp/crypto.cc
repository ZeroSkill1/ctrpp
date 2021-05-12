#include <ctrpp/crypto.hh>

using namespace ctrpp::exceptions::crypto;
using namespace ctrpp::exceptions::fs;

uint128_t rotl(uint128_t in, u32 bits)
{
	return ((in << bits) | (in >> (128 - bits)));
}

uint128_t rotr(uint128_t in, u32 bits)
{
	return ((in >> bits) | (in << (128 - bits)));
}

ctrpp::crypto::Engine::Engine()
{
}

ctrpp::crypto::Engine::Engine(const char *boot9_filename, bool dev)
{
	FILE *b9 = nullptr;

	long siz = 0;

	b9 = util::io::open_file(boot9_filename, "r", &siz);

	u8 hash[0x20];
	u8 b9_dat[B9_MINSIZE];

	this->is_dev = dev;

	u32 kblb_off = this->is_dev ? DEV_KEYBLOB_OFFSET : KEYBLOB_OFFSET;

	

	if (!(siz == B9_MINSIZE || siz == B9_MAXSIZE))
	{
		fclose(b9);
		throw InvalidFileSizeException(boot9_filename, siz, B9_MINSIZE, B9_MAXSIZE);
	}

	try
	{
		switch (siz)
		{
		case 0x8000:
			util::crypto::hash_file_part_read(b9, hash, b9_dat, 0x0, B9_MINSIZE);
			break;
		case 0x10000:
			util::crypto::hash_file_part_read(b9, hash, b9_dat, 0x8000, B9_MINSIZE);
			break;
		}
	}
	catch (std::exception &ex)
	{
		fclose(b9);
		throw;
	}

	fclose(b9);

	if (memcmp(B9_HASH, hash, 0x20) != 0)
	{
		throw HashMismatchException(boot9_filename);	
	}

	this->key_x = std::map<u8, uint128_t>();
	this->key_y = std::map<u8, uint128_t>();
	this->normal_key = std::map<u8, u8 *>();

	this->key_x[0x2C] = this->key_x[0x2D] = this->key_x[0x2E] = this->key_x[0x2F] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x170));
	this->key_x[0x30] = this->key_x[0x31] = this->key_x[0x32] = this->key_x[0x33] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x180));
	this->key_x[0x34] = this->key_x[0x35] = this->key_x[0x36] = this->key_x[0x37] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x190));
	this->key_x[0x38] = this->key_x[0x39] = this->key_x[0x3A] = this->key_x[0x3B] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x1A0));
	this->key_x[0x3C] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x1B0));
	this->key_x[0x3D] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x1C0));
	this->key_x[0x3E] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x1D0));
	this->key_y[0x04] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x1F0));
	this->key_y[0x06] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x210));
	this->key_y[0x07] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x220));
	this->key_y[0x08] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x230));
	this->key_y[0x09] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x240));
	this->key_y[0x0A] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x250));
	this->key_y[0x0B] = ctrpp::ints::to_be_u128(b9_dat + (kblb_off + 0x260));

	this->normal_key[0x0D] = new u8[KEY_SIZE];

	memcpy(this->normal_key[0x0D], b9_dat + (kblb_off + 0x270), KEY_SIZE);
}

void ctrpp::crypto::Engine::set_normal(u8 slot, uint128_t key)
{
	std::map<u8, u8 *>::iterator itr = this->normal_key.find(slot);

	if (itr != this->normal_key.end())
	{
		ctrpp::ints::to_be_bytes(key, itr->second);
		return;
	}

	u8 *set = new u8[KEY_SIZE];

	ctrpp::ints::to_be_bytes(key, set);

	this->normal_key[slot] = set;
}

void ctrpp::crypto::Engine::copy_key_to_normal(u8 slot, u8 *key)
{
	std::map<u8, u8 *>::iterator itr = this->normal_key.find(slot);

	if (itr != this->normal_key.end())
	{
		memcpy(itr->second, key, KEY_SIZE);
		return;
	}

	u8 *key_in = new u8[KEY_SIZE];

	memcpy(key_in, key, KEY_SIZE);

	this->normal_key[slot] = key_in;
}

void ctrpp::crypto::Engine::ctr_keygen(u8 slot, uint128_t key_x, uint128_t key_y)
{
	set_normal(slot, rotr(((rotl(key_x, 2) ^ key_y) + CTR_CONSTANT), 41));
}

void ctrpp::crypto::Engine::twl_keygen(u8 slot, uint128_t key_x, uint128_t key_y)
{
	set_normal(slot, rotl(((key_x ^ key_y) + TWL_CONSTANT), 42));
}

ctrpp::crypto::Engine::~Engine()
{
	for (std::map<u8, u8 *>::iterator i = this->normal_key.begin(); i != this->normal_key.end(); i++)
	{
		delete[] i->second;
	}
}