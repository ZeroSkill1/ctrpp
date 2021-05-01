#ifndef __ints_hh
#define __ints_hh

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef char16_t char16;

namespace ctrpp
{
	#if __BYTE__ORDER__ == __ORDER__LITTLE__
		#define _CTRPP_LE_U16(buffer) ((u16)buffer[1] << 8) | ((u16)buffer[0])
		#define _CTRPP_LE_U32(buffer) ((u32)buffer[3] << 24) | ((u32)buffer[2] << 16) | ((u32)buffer[1] << 8) | ((u32)buffer[0])
		#define _CTRPP_LE_U64(buffer) (((u64)buffer[7] << 56)) | (((u64)buffer[6] << 48)) | (((u64)buffer[5] << 40)) | (((u64)buffer[4] << 32)) | (((u64)buffer[3] << 24)) | (((u64)buffer[2] << 16)) | (((u64)buffer[1] << 8)) | ((u64)buffer[0])
		#define _CTRPP_LE_FLOAT(buffer) ((float)buffer[3] << 24) | ((float)buffer[2] << 16) | ((float)buffer[1] << 8) | ((float)buffer[0])

		#define _CTRPP_BE_U16(buffer) ((u16)buffer[0] << 8) | ((u16)buffer[1])
		#define _CTRPP_BE_U32(buffer) ((u32)buffer[0] << 24) | ((u32)buffer[1] << 16) | ((u32)buffer[2] << 8) | ((u32)buffer[3])
		#define _CTRPP_BE_U64(buffer) (((u64)buffer[0] << 56)) | (((u64)buffer[1] << 48)) | (((u64)buffer[2] << 40)) | (((u64)buffer[3] << 32)) | (((u64)buffer[4] << 24)) | (((u64)buffer[5] << 16)) | (((u64)buffer[6] << 8)) | ((u64)buffer[7])

		#define _CTRPP_BE_FLOAT(buffer) ((float)buffer[0] << 24) | ((float)buffer[1] << 16) | ((float)buffer[2] << 8) | ((float)buffer[3])
	#elif __BYTE__ODER__ == __ORDER__BIG__
		#define _CTRPP_LE_U16(buffer) ((u16)buffer[0] << 8) | ((u16)buffer[1])
		#define _CTRPP_LE_U32(buffer) ((u32)buffer[0] << 24) | ((u32)buffer[1] << 16) | ((u32)buffer[2] << 8) | ((u32)buffer[3])
		#define _CTRPP_LE_U64(buffer) (((u64)buffer[0] << 56)) | (((u64)buffer[1] << 48)) | (((u64)buffer[2] << 40)) | (((u64)buffer[3] << 32)) | (((u64)buffer[4] << 24)) | (((u64)buffer[5] << 16)) | (((u64)buffer[6] << 8)) | ((u64)buffer[7])

		#define _CTRPP_LE_FLOAT(buffer) ((float)buffer[0] << 24) | ((float)buffer[1] << 16) | ((float)buffer[2] << 8) | ((float)buffer[3])

		#define _CTRPP_BE_U16(buffer) ((u16)buffer[1] << 8) | ((u16)buffer[0])
		#define _CTRPP_BE_U32(buffer) ((u32)buffer[3] << 24) | ((u32)buffer[2] << 16) | ((u32)buffer[1] << 8) | ((u32)buffer[0])
		#define _CTRPP_BE_U64(buffer) (((u64)buffer[7] << 56)) | (((u64)buffer[6] << 48)) | (((u64)buffer[5] << 40)) | (((u64)buffer[4] << 32)) | (((u64)buffer[3] << 24)) | (((u64)buffer[2] << 16)) | (((u64)buffer[1] << 8)) | ((u64)buffer[0])

		#define _CTRPP_BE_FLOAT(buffer) ((float)buffer[3] << 24) | ((float)buffer[2] << 16) | ((float)buffer[1] << 8) | ((float)buffer[0])
	#else
		#error "Unsupported byte order"
	#endif
}

#endif