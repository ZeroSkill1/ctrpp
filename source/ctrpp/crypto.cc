#include <ctrpp/crypto.hh>

void ctrpp::crypto::print_error(int code)
{
	std::map<int, const char *>::const_iterator itr = err.find(code);
	
	if (itr != err.end())
		std::cerr << itr->second;
	else
		std::cerr << "Error code (" << code << ") has no description\n";
}

uint128_t get_key(u8 *dt, u8 *tmp, u32 offset)
{
	memcpy(tmp, dt + offset, KEY_SIZE);

	return ctrpp::ints::to_be_u128(tmp);
}

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
	u8 *hash = new u8[0x20];
	u8 *tmp_key = new u8[KEY_SIZE];
	u8 *key_0d = new u8[KEY_SIZE];
	u8 *b9_dat = new u8[B9_MINSIZE];

	bool file_open = false;

	if (dev)
		this->is_dev = true;

	u32 kblb_off = this->is_dev ? DEV_KEYBLOB_OFFSET : KEYBLOB_OFFSET;

	long siz = util::io::check_file(boot9_filename);

	if (siz == -1)
	{
		this->result = -1;
		goto failed;
	}

	if (!(siz == B9_MINSIZE || siz == B9_MAXSIZE))
	{
		this->result = -2;
		goto failed;
	}

	b9 = fopen64(boot9_filename, "r");

	if (b9 == nullptr)
	{
		this->result = -3;
		goto failed;
	}

	file_open = true;

	switch (siz)
	{
	case 0x8000:
		if (util::crypto::hash_file_part_read(b9, hash, b9_dat, 0x0, B9_MINSIZE) != 0)
		{
			this->result = -5;
			goto failed;
		}
		break;
	case 0x10000:
		if (util::crypto::hash_file_part_read(b9, hash, b9_dat, 0x8000, B9_MINSIZE) != 0)
		{
			this->result = -5;
			goto failed;
		}
		break;
	default:
		this->result = -4;
		goto failed;
	}

	fclose(b9);
	file_open = false;

	if (memcmp(B9_HASH, hash, 0x20) != 0)
	{
		this->result = -6;
		goto failed;
	}

	this->key_x[0x2C] = this->key_x[0x2D] = this->key_x[0x2E] = this->key_x[0x2F] = get_key(b9_dat, tmp_key, kblb_off + 0x170);
	this->key_x[0x30] = this->key_x[0x31] = this->key_x[0x32] = this->key_x[0x33] = get_key(b9_dat, tmp_key, kblb_off + 0x180);
	this->key_x[0x34] = this->key_x[0x35] = this->key_x[0x36] = this->key_x[0x37] = get_key(b9_dat, tmp_key, kblb_off + 0x190);
	this->key_x[0x38] = this->key_x[0x39] = this->key_x[0x3A] = this->key_x[0x3B] = get_key(b9_dat, tmp_key, kblb_off + 0x1A0);
	this->key_x[0x3C] = get_key(b9_dat, tmp_key, kblb_off + 0x1B0);
	this->key_x[0x3D] = get_key(b9_dat, tmp_key, kblb_off + 0x1C0);
	this->key_x[0x3E] = get_key(b9_dat, tmp_key, kblb_off + 0x1D0);
	this->key_y[0x04] = get_key(b9_dat, tmp_key, kblb_off + 0x1F0);
	this->key_y[0x06] = get_key(b9_dat, tmp_key, kblb_off + 0x210);
	this->key_y[0x07] = get_key(b9_dat, tmp_key, kblb_off + 0x220);
	this->key_y[0x08] = get_key(b9_dat, tmp_key, kblb_off + 0x230);
	this->key_y[0x09] = get_key(b9_dat, tmp_key, kblb_off + 0x240);
	this->key_y[0x0A] = get_key(b9_dat, tmp_key, kblb_off + 0x250);
	this->key_y[0x0B] = get_key(b9_dat, tmp_key, kblb_off + 0x260);

	memcpy(key_0d, b9_dat + (kblb_off + 0x270), KEY_SIZE);

	this->normal_key[0x0D] = key_0d;

	for (std::map<u8, uint128_t>::iterator i = this->key_x.begin(); i != this->key_x.end(); i++)
	{
		printf("KeyX Slot 0x%02X : %s\n", i->first, util::buffer_to_hex(BYTES(i->second), sizeof(uint128_t), true).c_str());
	}

	for (std::map<u8, uint128_t>::iterator i = this->key_y.begin(); i != this->key_y.end(); i++)
	{
		printf("KeyY Slot 0x%02X : %s\n", i->first, util::buffer_to_hex(BYTES(i->second), sizeof(uint128_t), true).c_str());
	}

	printf("NormalKey Slot 0x%02X : %s\n", 0x0D, util::buffer_to_hex(this->normal_key[0x0D], sizeof(uint128_t), true).c_str());

	this->result = 0;
	delete[] tmp_key;
	delete[] b9_dat;
	delete[] hash;
	return;

failed:

	if (file_open)
		fclose(b9);

	delete[] tmp_key;
	delete[] b9_dat;
	delete[] hash;
	delete[] key_0d;
	return;
}

// void ctrpp::crypto::Engine::set_normal(u8 slot, uint128_t key)
// {
// 	std::map<u8, u8 *>::iterator itr = this->normal_key.find(slot);

// 	if (itr != this->normal_key.end())
// 	{
// 		ctrpp::ints::to_be_bytes(key, itr->second);
// 		return;
// 	}

// 	u8* set = new u8[KEY_SIZE];

// 	ctrpp::ints::to_be_bytes(key, set);

// 	this->normal_key[slot] = set;
// }

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

// void ctrpp::crypto::Engine::ctr_keygen(u8 slot, uint128_t key_x, uint128_t key_y)
// {
// 	set_normal(slot, rotr(((rotl(key_x, 2) ^ key_y) + CTR_CONSTANT ), 41));
// }

// void ctrpp::crypto::Engine::twl_keygen(u8 slot, uint128_t key_x, uint128_t key_y)
// {
// 	set_normal(slot, rotl(((key_x ^ key_y) + TWL_CONSTANT), 42));
// }

ctrpp::crypto::Engine::~Engine()
{
	for (std::map<u8, u8 *>::iterator i = this->normal_key.begin(); i != this->normal_key.end(); i++)
	{
		delete [] i->second;
	}
}