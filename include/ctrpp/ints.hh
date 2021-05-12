#ifndef __ints_hh
#define __ints_hh

#include <stdint.h>
#include <string.h>

#include <uint128_t/uint128_t.hh>
#include <ctrpp/constants.hh>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef char16_t char16;

namespace ctrpp
{
#define BYTES(x) ((u8 *)(&x))
#define BYTES_PTR(x) ((u8 *)x)

#if __BYTE__ORDER__ == __ORDER__LITTLE__

#define _CTRPP_LE_U16(buffer) \
( \
 ((u16)buffer[1] << 8) | \
 ((u16)buffer[0]) \
)

#define _CTRPP_LE_U32(buffer) \
( \
 ((u32)buffer[3] << 24) | \
 ((u32)buffer[2] << 16) | \
 ((u32)buffer[1] << 8) | \
 ((u32)buffer[0]) \
)

#define _CTRPP_LE_U64(buffer) \
( \
 ((u64)buffer[7] << 56) | \
 ((u64)buffer[6] << 48) | \
 ((u64)buffer[5] << 40) | \
 ((u64)buffer[4] << 32) | \
 ((u64)buffer[3] << 24) | \
 ((u64)buffer[2] << 16) | \
 ((u64)buffer[1] << 8) | \
 ((u64)buffer[0]) \
)
#define _CTRPP_BE_U16(buffer) \
( \
 ((u16)buffer[0] << 8) | \
 ((u16)buffer[1]) \
)

#define _CTRPP_BE_U32(buffer) \
(\
 ((u32)buffer[0] << 24) | \
 ((u32)buffer[1] << 16) | \
 ((u32)buffer[2] << 8) | \
 ((u32)buffer[3]) \
)

#define _CTRPP_BE_U64(buffer) \
( \
 ((u64)buffer[0] << 56) | \
 ((u64)buffer[1] << 48) | \
 ((u64)buffer[2] << 40) | \
 ((u64)buffer[3] << 32) | \
 ((u64)buffer[4] << 24) | \
 ((u64)buffer[5] << 16) | \
 ((u64)buffer[6] << 8) | \
 ((u64)buffer[7]) \
)

#elif __BYTE__ODER__ == __ORDER__BIG__
#define _CTRPP_LE_U16(buffer) \
( \
 ((u16)buffer[0] << 8) | \
 ((u16)buffer[1]) \
)

#define _CTRPP_LE_U32(buffer) \
(\
 ((u32)buffer[0] << 24) | \
 ((u32)buffer[1] << 16) | \
 ((u32)buffer[2] << 8) | \
 ((u32)buffer[3]) \
)

#define _CTRPP_LE_U64(buffer) \
( \
 ((u64)buffer[0] << 56) | \
 ((u64)buffer[1] << 48) | \
 ((u64)buffer[2] << 40) | \
 ((u64)buffer[3] << 32) | \
 ((u64)buffer[4] << 24) | \
 ((u64)buffer[5] << 16) | \
 ((u64)buffer[6] << 8) | \
 ((u64)buffer[7]) \
)

#define _CTRPP_BE_U16(buffer) \
( \
 ((u16)buffer[1] << 8) | \
 ((u16)buffer[0]) \
)

#define _CTRPP_BE_U32(buffer) \
( \
 ((u32)buffer[3] << 24) | \
 ((u32)buffer[2] << 16) | \
 ((u32)buffer[1] << 8) | \
 ((u32)buffer[0]) \
)

#define _CTRPP_BE_U64(buffer) \
( \
 ((u64)buffer[7] << 56) | \
 ((u64)buffer[6] << 48) | \
 ((u64)buffer[5] << 40) | \
 ((u64)buffer[4] << 32) | \
 ((u64)buffer[3] << 24) | \
 ((u64)buffer[2] << 16) | \
 ((u64)buffer[1] << 8) | \
 ((u64)buffer[0]) \
)

#else
#error "Unsupported byte order"
#endif

	namespace ints
	{
		float le_float(u8 *buf);
		float be_float(u8 *buf);
		void to_le_bytes(uint128_t in, u8 *buf);
		void to_be_bytes(uint128_t in, u8 *buf);
		uint128_t to_le_u128(u8 *buf);
		uint128_t to_be_u128(u8 *buf);
	}
}

#endif