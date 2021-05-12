#ifndef __crypto_hh
#define __crypto_hh

#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>

#include <map>
#include <bit>

#include <ctrpp/exceptions.hh>
#include <ctrpp/ints.hh>
#include <ctrpp/util/io.hh>

namespace ctrpp
{
	namespace crypto
	{
		static const u8 B9_HASH[0x20] = {0x73, 0x31, 0xF7, 0xED, 0xEC, 0xE3, 0xDD, 0x33, 0xF2, 0xAB, 0x4B, 0xD0, 0xB3, 0xA5, 0xD6, 0x07, 0x22, 0x9F, 0xD1, 0x92, 0x12, 0xC1, 0x0B, 0x73, 0x4C, 0xED, 0xCA, 0xF7, 0x8C, 0x1A, 0x7B, 0x98};
		static const u8 DEV_COMMON_KEY_0[0x10] = {0x55, 0xA3, 0xF8, 0x72, 0xBD, 0xC8, 0x0C, 0x55, 0x5A, 0x65, 0x43, 0x81, 0x13, 0x9E, 0x15, 0x3B};

		static const uint128_t CTR_CONSTANT = uint128_t("42503689118608475533858958821215598218");
		static const uint128_t TWL_CONSTANT = uint128_t("340277079404625077361735292783915122297");

		enum class Keyslot
		{
			TWLNAND1          = 0x00,
			TWLNAND2          = 0x01,
			TWLNAND3          = 0x02,
			TWLNAND4          = 0x03,
			CTRNAND           = 0x04,
			KTRNAND           = 0x05,
			FIRM              = 0x06,
			AGBSAVE           = 0x07,
			CMAC_NAND_DB      = 0x0B,
			NCCH_9_3          = 0x18,
			CMAC_CARDSAVE_NEW = 0x19,
			CARDSAVE_NEW      = 0x1A,
			NCCH_9_6          = 0x1B,
			CMAC_AGB          = 0x24,
			NCCH_7_0          = 0x25,
			NCCH              = 0x2C,
			UDS_LOCAL_WLAN    = 0x2D,
			CEC               = 0x2E,
			SAVE_6_0          = 0x2F,
			CMAC_SD_NAND      = 0x30,
			CMAC_CARD_SAVE    = 0x33,
			SD                = 0x34,
			CARD_SAVE         = 0x37,
			BOSS              = 0x38,
			DLP               = 0x39,
			TWL_EXPORT        = 0x3A,
			COMMON_KEY        = 0x3D,
			BOOT9_INTERNAL    = 0x3F
		};

		class Engine
		{
		public:
			std::map<u8, uint128_t> key_y;
			std::map<u8, uint128_t> key_x;
			std::map<u8, u8 *> normal_key;

			bool is_dev = false;
			int result;

			Engine();
			Engine(const char *boot9_filename, bool dev = false);

			void ctr_keygen(u8 slot, uint128_t key_x, uint128_t key_y);
			void twl_keygen(u8 slot, uint128_t key_x, uint128_t key_y);
			void copy_key_to_normal(u8 slot, u8 *key);

			~Engine();

		private:
			void set_normal(u8 slot, uint128_t key);
		};
	}
}

#endif