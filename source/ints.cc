#include <ints.hh>

#if __BYTE__ORDER__ == __ORDER__LITTLE__
	float ctrpp::ints::le_float(u8 *buf)
	{
		float f = 0.0F;

		(&f)[0] = buf[0];
		(&f)[1] = buf[1];
		(&f)[2] = buf[2];
		(&f)[3] = buf[3];

		return f;
	}

	float ctrpp::ints::be_float(u8 *buf)
	{
		float f = 0.0F;

		(&f)[0] = buf[3];
		(&f)[1] = buf[2];
		(&f)[2] = buf[1];
		(&f)[3] = buf[0];

		return f;
	}

#elif __BYTE__ORDER__ == __ORDER__BIG__
	float ctrpp::ints::le_float(u8 *buf)
	{
		float f = 0.0F;

		(&f)[0] = buf[3];
		(&f)[1] = buf[2];
		(&f)[2] = buf[1];
		(&f)[3] = buf[0];

		return f;
	}

	float ctrpp::ints::be_float(u8 *buf)
	{
		float f = 0.0F;

		(&f)[0] = buf[0];
		(&f)[1] = buf[1];
		(&f)[2] = buf[2];
		(&f)[3] = buf[3];

		return f;
	}
#else
	#error "Unsupported byte order"
#endif