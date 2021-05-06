#include <ctrpp/ints.hh>

float ctrpp::ints::le_float(u8 *buf)
{
	float f = 0.0F;

#if __BYTE__ORDER == __ORDER__LITTLE__
	(&f)[0] = buf[0];
	(&f)[1] = buf[1];
	(&f)[2] = buf[2];
	(&f)[3] = buf[3];
#elif __BYTE__ORDER__ == __ORDER__BIG__
	(&f)[0] = buf[3];
	(&f)[1] = buf[2];
	(&f)[2] = buf[1];
	(&f)[3] = buf[0];
#else
#error "Unsupported byte order"
#endif

	return f;
}

float ctrpp::ints::be_float(u8 *buf)
{
	float f = 0.0F;

#if __BYTE__ORDER == __ORDER__LITTLE__
	(&f)[0] = buf[3];
	(&f)[1] = buf[2];
	(&f)[2] = buf[1];
	(&f)[3] = buf[0];
#elif __BYTE__ORDER__ == __ORDER__BIG__
	(&f)[0] = buf[0];
	(&f)[1] = buf[1];
	(&f)[2] = buf[2];
	(&f)[3] = buf[3];
#else
#error "Unsupported byte order"
#endif

	return f;
}

uint128_t ctrpp::ints::to_le_u128(u8 *buf)
{
#if __BYTE__ORDER__ == __ORDER__LITTLE__
	u64 upper = _CTRPP_LE_U64((buf + 8));
	u64 lower = _CTRPP_LE_U64(buf);
#elif
	u64 upper = _CTRPP_LE_U64(buf);
	u64 lower = _CTRPP_LE_U64(buf + 8);
#else
#error "Unsupported byte order"
#endif

	return uint128_t(upper, lower);
}

uint128_t ctrpp::ints::to_be_u128(u8 *buf)
{
#if __BYTE__ORDER__ == __ORDER__LITTLE__
	u64 upper = _CTRPP_BE_U64((buf + 8));
	u64 lower = _CTRPP_BE_U64(buf);
#elif
	u64 upper = _CTRPP_BE_U64(buf);
	u64 lower = _CTRPP_BE_U64(buf + 8);
#else
#error "Unsupported byte order"
#endif

	return uint128_t(lower, upper);
}