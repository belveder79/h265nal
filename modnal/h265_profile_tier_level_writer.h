#ifndef H265_PROFILE_TIER_LEVEL_WRITER_INCLUDED
#define H265_PROFILE_TIER_LEVEL_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteProfileInfo(H265ProfileInfoParser::ProfileInfoState* profile_info, BitBufferWriter* bit_buffer) noexcept;
bool WriteProfileTierLevel(H265ProfileTierLevelParser::ProfileTierLevelState* profile_tier_level, BitBufferWriter* bit_buffer) noexcept;

#endif // H265_PROFILE_TIER_LEVEL_WRITER_INCLUDED
