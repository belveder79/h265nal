#ifndef H265_SPS_RANGE_EXTENSION_WRITER_INCLUDED
#define H265_SPS_RANGE_EXTENSION_WRITER_INCLUDED

#include <h265_bitstream_parser.h>
#include <h265_common.h>
#include "h265_wutils.h"


bool WriteSpsRangeExtension(const H265SpsRangeExtensionParser::SpsRangeExtensionState* sps_range_extension,
                            BitBufferWriter* bit_buffer) noexcept;

#endif // H265_SPS_RANGE_EXTENSION_WRITER_INCLUDED
