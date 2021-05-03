#ifndef __smdh_hh
#define __smdh_hh

#include <string.h>
#include <stdio.h>

#include <ints.hh>
#include <util.hh>

namespace ctrpp
{
	namespace types
	{
		namespace SMDH
		{
			enum class SMDHRegionLockout
			{
				Japan = 0x1,
				USA = 0x2,
				Europe = 0x4,
				Australia = 0x8,
				China = 0x10,
				Korea = 0x20,
				Taiwan = 0x40
			};

			enum class SMDHFlags
			{
				VisibleOnHomeMenu = 0x1,
				AutoBootGamecard = 0x2,
				AllowUseOf3d = 0x4,
				RequireAcceptCtrEula = 0x8,
				AutoSaveOnExit = 0x10,
				UsesExtendedBanner = 0x20,
				RegionGameRatingRequired = 0x40,
				UsesSaveData = 0x80,
				RecordUsageData = 0x100,
				DisableSdSaveDataBackups = 0x400,
				New3dsExclusive = 0x1000
			};

			typedef struct __attribute__((packed)) smdh_title_name
			{
				char16 short_desc[0x40];
				char16 long_desc[0x80];
				char16 publisher[0x40];
			} smdh_title_name;

			typedef struct __attribute__((packed)) smdh_data
			{
				char magic[4];
				u16 version;
				u16 reserved;
				smdh_title_name title_names[16];

				//application settings

				u8 cero_rating_flag;
				u8 esrb_rating_flag;
				u8 reserved_1;
				u8 usk_rating_flag;
				u8 pegi_gen_rating_flag;
				u8 reserved_2;
				u8 pegi_prt_rating_flag;
				u8 pegi_bbfc_rating_flag;
				u8 cob_rating_flag;
				u8 grb_rating_flag;
				u8 cgsrr_rating_flag;
				u8 reserved_3;
				u8 reserved_4;
				u8 reserved_5;
				u8 reserved_6;
				u8 reserved_7;
				u32 region_lockout; //LE
				u32 match_maker_id;
				u64 match_maker_bit_id;
				u32 flags; //LE
				u16 eula_version;
				u16 reserved_8;
				float default_banner_animation_frame;
				u32 cec_id;

				u64 reserved_9;

				// icons
				u16 small_icon_data[576];
				u16 large_icon_data[2304];
			} smdh_data;

			class SMDH
			{
			public:
				SMDH();
				SMDH(const char *filename);

				bool success_parsed;

				smdh_data *raw_smdh_data = nullptr;

				u32 region_lockout();
				u32 match_maker_id();
				u64 match_maker_bit_id();
				u16 eula_version();
				float default_banner_animation_frame();
				u32 cec_id();

				~SMDH();
			};
		}
	}
}

#endif