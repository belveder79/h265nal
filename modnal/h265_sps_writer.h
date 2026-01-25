#ifndef H265_SPS_WRITER_INCLUDED
#define H265_SPS_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteSps(h265nal::H265SpsParser::SpsState* vps, h265nal::BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SPS_WRITER_INCLUDED
