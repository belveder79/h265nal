#ifndef H265_VPS_WRITER_INCLUDED
#define H265_VPS_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"

bool WriteVps(h265nal::H265VpsParser::VpsState* vps, h265nal::BitBufferWriter* bit_buffer) noexcept;

#endif // H265_VPS_WRITER_INCLUDED
