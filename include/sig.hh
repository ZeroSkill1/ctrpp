#ifndef __sig_hh
#define __sig_hh

#include <ints.hh>

namespace ctrpp
{
	enum class SignatureType
	{
		RSA_4096_SHA1 = 0x00010000,
		RSA_2048_SHA1 = 0x00010001,
		ECDSA_SHA1 = 0x00010002,
		RSA_4096_SHA256 = 0x00010003,
		RSA_2048_SHA256 = 0x00010004,
		ECDSA_SHA256 = 0x00010005
	};

	enum class SignatureSize
	{
		RSA_4096_SHA1 = 0x200,
		RSA_2048_SHA1 = 0x100,
		ECDSA_SHA1 = 0x03C,
		RSA_4096_SHA256 = 0x200,
		RSA_2048_SHA256 = 0x100,
		ECDSA_SHA256 = 0x03C
	};

	enum class SignaturePaddingSize
	{
		RSA_4096_SHA1 = 0x3C,
		RSA_2048_SHA1 = 0x3C,
		ECDSA_SHA1 = 0x40,
		RSA_4096_SHA256 = 0x3C,
		RSA_2048_SHA256 = 0x3C,
		ECDSA_SHA256 = 0x40
	};

	class Signature
	{
	public:
		SignatureType sig_type;
		u32 sig_size;
		u32 sig_padding_size;
		u32 full_size;

		u8 *sig = nullptr;
		u8 *sig_padding = nullptr;

		Signature();
		Signature(u32 sig_type);
		~Signature();
	};
}

#endif