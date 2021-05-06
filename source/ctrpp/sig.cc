#include <string.h>
#include <stdio.h>

#include <ctrpp/sig.hh>

ctrpp::Signature::Signature()
{
}

ctrpp::Signature::Signature(u32 sig_type)
{
	switch (sig_type)
	{
	case (u32)SignatureType::RSA_4096_SHA1:
		this->sig_type = SignatureType::RSA_4096_SHA1;
		this->sig_padding_size = (u32)SignaturePaddingSize::RSA_4096_SHA1;
		this->sig_size = (u32)SignatureSize::RSA_4096_SHA1;
		break;
	case (u32)SignatureType::RSA_2048_SHA1:
		this->sig_type = SignatureType::RSA_2048_SHA1;
		this->sig_padding_size = (u32)SignaturePaddingSize::RSA_2048_SHA1;
		this->sig_size = (u32)SignatureSize::RSA_2048_SHA1;
		break;
	case (u32)SignatureType::ECDSA_SHA1:
		this->sig_type = SignatureType::ECDSA_SHA1;
		this->sig_padding_size = (u32)SignaturePaddingSize::ECDSA_SHA1;
		this->sig_size = (u32)SignatureSize::ECDSA_SHA1;
		break;
	case (u32)SignatureType::RSA_4096_SHA256:
		this->sig_type = SignatureType::RSA_4096_SHA256;
		this->sig_padding_size = (u32)SignaturePaddingSize::RSA_4096_SHA256;
		this->sig_size = (u32)SignatureSize::RSA_4096_SHA256;
		break;
	case (u32)SignatureType::RSA_2048_SHA256:
		this->sig_type = SignatureType::RSA_2048_SHA256;
		this->sig_padding_size = (u32)SignaturePaddingSize::RSA_2048_SHA256;
		this->sig_size = (u32)SignatureSize::RSA_2048_SHA256;
		break;
	case (u32)SignatureType::ECDSA_SHA256:
		this->sig_type = SignatureType::ECDSA_SHA256;
		this->sig_padding_size = (u32)SignaturePaddingSize::ECDSA_SHA256;
		this->sig_size = (u32)SignatureSize::ECDSA_SHA256;
		break;
	default:
		this->full_size = 0;
		return;
	}

	this->full_size = 4 + this->sig_size + this->sig_padding_size;

	this->sig = new u8[this->sig_size];
	this->sig_padding = new u8[this->sig_padding_size];
}

ctrpp::Signature::~Signature()
{
	if (this->sig != nullptr)
	{
		delete[] this->sig;
		delete[] this->sig_padding;
	}
};