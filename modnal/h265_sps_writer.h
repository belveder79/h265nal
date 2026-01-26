#ifndef H265_SPS_WRITER_INCLUDED
#define H265_SPS_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSps(H265SpsParser::SpsState* vps, BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SPS_WRITER_INCLUDED
